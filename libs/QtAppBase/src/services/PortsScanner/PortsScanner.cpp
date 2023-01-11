#include "PortsScanner.h"

namespace Services { namespace PortsScanner
{

PortsScanner::PortsScanner(int max_threads_count)
{   
    if( max_threads_count <= 1 || max_threads_count >= 500 )
    {
        qDebug() << "Max threads count out of range for port scanner";
        return;
    }

    this->ThreadsPoolPtr()->setMaxThreadCount(max_threads_count);
}

bool PortsScanner::EnqueueScan(const QString &host, const QString &scanProfileName)
{
    this->CancelAllThreads = 0;

    // Already running a scan for this host?
    if( this->IsScanRunning(host) )
    {
        PortsScanResult response;
        response.AppErrorDetected = true;
        response.AppErrorDesc = "";

        emit this->OnRequestError(host, response);
        return false;
    }

    // Add record to queue to be executed by the threads in pool - this will be executed from a separate thread
    auto lam = [this, host, scanProfileName]()
    {
        emit(this->AvailableWorkersChanged(this->ThreadsPoolPtr()->AvailableThreads(), this->ThreadsPoolPtr()->ActiveThreads()));
        this->Task(host, scanProfileName);
        emit(this->AvailableWorkersChanged(this->ThreadsPoolPtr()->AvailableThreads(), this->ThreadsPoolPtr()->ActiveThreads()-1)); // substract current worker which will be disposed
    };

    if(!this->ThreadsPoolPtr()->tryStart(lam))
    {
        PortsScanResult response;
        response.AppErrorDetected = true;
        response.AppErrorDesc = "No threads available";

        emit this->OnRequestError(host, response);
        return false;
    }

    this->mutex.lock();
    ActiveHostsScanned.append(host);
    this->mutex.unlock();
    return true;
}

void PortsScanner::RequestAllJobsStop()
{
    this->CancelAllThreads = 1;
}

bool PortsScanner::IsScanRunning(const QString &host)
{
    QMutexLocker locker(&mutex);

    if( this->ActiveHostsScanned.contains(host) )
        return true;
    return false;
}

void PortsScanner::Task(const QString &host, const QString &scanProfileName)
{
    // Create output var
    PortsScanResult output;
    output.ScanProfile = scanProfileName;
    output.StartScanTimestamp = QDateTime::currentSecsSinceEpoch();

    // Emit a notification when scan started
    emit this->OnRequestStarted(host, output);

    // Check if host is up and if XML returned by nmap does not indicate any error
    if( this->RunNmapPingAndRDNSScan(host, &output) )
    {
        // Notify UI about the progress done
        emit this->OnProcessProgress(host, output);

        // Read scan profile
        PortsScanProfileType scanProfile = PortsScanProfilesManager::instance().GetByName(scanProfileName);

        // Currently, is launched one nmap scan for each target within profile
        for (PortsScanTargetType &target: scanProfile.Targets)
        {
            if(this->CancelAllThreads > 0)
            {
                output.AppErrorDetected = true;
                output.AppErrorDesc = "Canceled by user";
                break;
            }

            // If request only have UDP ports and UDP is disables, just skip this
            if( this->DisableUdpScan )
            {
                if( target.TcpPorts.count() == 0 && target.UdpPorts.count() > 0 )
                    continue;
            }

            // Build nMap command string
            QString scanRequestCommandString = BuildNmapScanCommand(host, target);

            // Launch nmap scan
            QString nmapOutputXml = this->RunNmapScan(scanRequestCommandString);

            // Check if nmap output is valid
            nMapXmlParser xmlParser;
            if( !this->TryParseNmapXML(nmapOutputXml, &xmlParser, &output, scanRequestCommandString))
            {
                break;
            }

            output.TargetsOutputs.append(xmlParser.GetInputXML());

            // Update portslist scanned
            QList<nMapPortState> ports = xmlParser.GetNmapParam_OpenPorts();
            for( const nMapPortState &port: ports )
            {
                if (port.Protocol.toLower() == "tcp")
                    output.OpenTcpPorts.append(QString::number(port.PortNumber));
                else
                    output.OpenUdpPorts.append(QString::number(port.PortNumber));
            }

            // Notify UI about the progress done
            emit this->OnProcessProgress(host, output);
        }
    }

    // Remove ports duplicates
    output.OpenTcpPorts.removeDuplicates();
    output.OpenUdpPorts.removeDuplicates();
    // Sort list
    Utils_NumericListSort(&output.OpenTcpPorts);
    Utils_NumericListSort(&output.OpenUdpPorts);

    // Calculate how many seconds all scan took
    output.ScanDurationSeconds = QDateTime::currentSecsSinceEpoch() - output.StartScanTimestamp;

    // Emit finishing signals
    if(output.NetworkErrorDetected || output.AppErrorDetected )
    {
        emit this->OnRequestError(host, output);
    }
    else
    {
        emit this->OnRequestFinished(host, output);
    }

    this->mutex.lock();
    this->ActiveHostsScanned.removeAll(host);
    this->mutex.unlock();
}

QString PortsScanner::BuildNmapScanCommand(const QString &host, PortsScanTargetType &target)
{
    QString output = "nmap ";

    // Add TCP/UDP scan flags
    output += (target.TcpPorts.count() > 0)?"-sT ":"";

    if( !this->DisableUdpScan )
        output += (target.UdpPorts.count() > 0)?"-sU ":"";

    // Add flag to specify the ports
    if( !this->DisableUdpScan )
    {
        output += ((target.TcpPorts.count() > 0) || (target.UdpPorts.count() > 0))?"-p":"";
    }
    else
    {
        output += (target.TcpPorts.count() > 0)?"-p":"";
    }

    // Add tcp ports
    if(target.TcpPorts.count() > 0)
    {
        output += "T:" + target.GetTcpPortsString();
    }

    if( !this->DisableUdpScan )
    {
        if(target.UdpPorts.count() > 0)
        {
            output += target.TcpPorts.count() > 0 ? "," : "";
            output += "U:";
            output += target.GetUdpPortsString();
        }
    }

    // Append whitespace after ports, if any were specified
    if( !this->DisableUdpScan )
    {
        output += ((target.TcpPorts.count() > 0) || (target.UdpPorts.count() > 0))?" ":"";
    }
    else
    {
        output += (target.TcpPorts.count() > 0)?" ":"";
    }


    if( !target.nMapArguments.isEmpty() )
    {
        // IF scripts are provided, look first in the custom location and only then try to load default scripts
        // To give the user posibility to load the scripts it wants to
        if( target.nMapArguments.contains("--script") )
        {
            QString scriptsStrOrig = target.nMapArguments.split("--script")[1].split(" ")[0];
            QString scriptsStr = scriptsStrOrig.trimmed();
            if (scriptsStr.startsWith("="))
            {
                scriptsStr = scriptsStr.remove(0, 1).trimmed();
            }

            // Now we have the list with scripts provided with proper extension.
            QStringList outputScriptsList;
            QStringList scriptsList = scriptsStr.split(",");
            for(QString &script: scriptsList)
            {
                if( !script.endsWith(".nse") )
                    script += ".nse";

                // If script already exists, full path was provided. Leave it as it is
                if(Utils_FileExists(script) )
                {
                    outputScriptsList.append(script);
                    continue;
                }

                // Check user directory for nMap scripts
                bool found = false;
                for(QString user_Loc: Settings::GetUserDataPaths())
                {
                    if(Utils_FileExists(user_Loc + "/nmap_scripts/" + script) )
                    {
                        outputScriptsList.append(user_Loc + "/nmap_scripts/" + script);
                        found  = true;
                        break;
                    }
                }
                if( found )
                    continue;

                // Check if script is located in default directory
                if(Utils_FileExists(QCoreApplication::applicationDirPath() + "/data/nmap_scripts/" + script) )
                {
                    outputScriptsList.append(QCoreApplication::applicationDirPath() + "/data/nmap_scripts/" + script);
                    continue;
                }

                // Script not found, just provide it to nMap as an argument as it is
                outputScriptsList.append(script);
            }

            // Set scripts back
            QString scriptsArg = "=" + outputScriptsList.join(",");
            QString newNmapCommandArgs = target.nMapArguments.replace(scriptsStrOrig, scriptsArg);
            output += newNmapCommandArgs + " ";
        }
        else
        {
            output += target.nMapArguments + " ";
        }
    }

    output += host + " " + "-oX -";

    return output;
}

QString PortsScanner::RunNmapScan(QString nMapCommand)
{
    //qDebug() << "Execute " << nMapCommand;
    QStringList args = nMapCommand.split(" ");
    QString nmapExecutable = args[0];
    args.pop_front();
    
    if( args.isEmpty() || nmapExecutable.isEmpty() )
        return "nmap command empty";
    
    // Use openssl to parse certificate
    QProcess process;
    process.start(nmapExecutable, args);
    process.waitForFinished(999999999);
    QString processOutput = process.readAllStandardOutput();
    processOutput += process.readAllStandardError();

    // If output contain "wpcap.dll", then remove first line. This is not really a fatal error and prevents other things to be done
    if( processOutput.contains("wpcap.dll") )
    {
        QString line = processOutput.split('\n')[0];
        processOutput = QString(processOutput).replace(line + "\n", "");
    }

    return processOutput;
}

bool PortsScanner::RunNmapPingAndRDNSScan(const QString &host, PortsScanResult *output)
{
    QString scanCommand = "nmap -sn " + host + " -oX -";
    QString scanXmlnMap = this->RunNmapScan(scanCommand);

    nMapXmlParser xmlParser;
    if( !this->TryParseNmapXML(scanXmlnMap, &xmlParser, output, scanCommand))
    {
        output->PingOutput = scanXmlnMap;
        if (Utils_IsValidIPv4(host))
        {
            output->HostIp = host;
        }
        else
        {
            output->HostRdns = host;
        }

        return false;
    }

    output->HostIp = xmlParser.GetNmapParam_TargetAddress();
    output->HostRdns = xmlParser.GetNmapParam_TargetRDNS();
    output->PingState = xmlParser.GetNmapParam_TargetState();
    output->PingLatencySeconds = xmlParser.GetNmapParam_LatencySeconds();
    output->PingOutput = xmlParser.GetInputXML();

    if( output->HostIp.isEmpty() && Utils_IsValidIPv4(host))
    {
        output->HostIp = host;
    }

    return true;
}

bool PortsScanner::TryParseNmapXML(QString xmlContent, nMapXmlParser *outputNmapXmlParser, PortsScanResult *output, QString scanCommand)
{
    if( !outputNmapXmlParser->ParseXML(xmlContent))
    {
        QString msg = "nMap XML error: " + outputNmapXmlParser->GetParsingErrorDesc() + ". Command " + scanCommand;
        qDebug() << msg;
        output->AppErrorDetected = true;
        output->AppErrorDesc = msg;
        output->TargetsOutputs.append(xmlContent);
        return false;
    }

    // nMap stopped with errors?
    if( !outputNmapXmlParser->GetNmapParam_ScanSucceed() )
    {
        QString msg = "nMap exit with error code";
        qDebug() << msg + ". Command " + scanCommand;
        output->AppErrorDetected = true;
        output->AppErrorDesc = msg;
        output->TargetsOutputs.append(outputNmapXmlParser->GetInputXML());
        return false;
    }

    return true;
}

}} // Namespaces



