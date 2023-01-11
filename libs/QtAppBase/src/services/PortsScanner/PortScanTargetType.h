#ifndef _SERVICES_PORTSCANTARGETTYPE_H_
#define _SERVICES_PORTSCANTARGETTYPE_H_

#include <QString>
#include <QList>
#include <QMap>
#include <QStringList>

#include "utils/utils.h"

namespace Services { namespace PortsScanner
{

class DeviceTypes
{
public:
    const QString ICS = "ICS";
    const QString WEB = "WEB";
};

class PortsScanTargetType
{
public:
    QString TargetType;
    QString TargetName;
    QList<quint16> TcpPorts;
    QList<quint16> UdpPorts;
    QStringList ShodanDorks;
    QString nMapArguments;
    QString Description;
    QString GitSrc;

    bool ParseTargetFromString(QString input)
    {
        if( input.isEmpty() )
            return false;

        // 8 separators needs to be present
        QStringList elements = Utils_ParseCsvLine(input);

        if( elements.count() < 8 )
        {
            qDebug() << "Error while decoding target from string. Too few elements: " << elements;
            return false;
        }

        int counter = 0;

        // Parse category, device and vendor info
        this->TargetType = elements[counter++];
        this->TargetName = elements[counter++];

        // Parse TCP ports
        this->TcpPorts = this->ParsePorts(elements[counter++]);

        // Parse UDP ports
        this->UdpPorts = this->ParsePorts(elements[counter++]);

        // Parse shodan dorks
        this->ShodanDorks = this->ParseShodanDorks(elements[counter++]);

        // nMap arguments to be used during scan
        this->nMapArguments = elements[counter++];

        // Parse description
        this->Description = elements[counter++];

        // Git source
        this->GitSrc = elements[counter++];

        return true;
    }

    QString Serialize()
    {
        return "";
    }

    QString ToString()
    {
        return "Target type: " + this->TargetType
        + ", Target name: " + this->TargetName
        + ", TCP: " + this->GetPortsString(true)
        + ", UDP: " + this->GetPortsString(false)

        + ", Description: " + this->Description
        ;
    }

    QString GetTcpPortsString()
    {
        return this->GetPortsString(true);
    }

    QString GetUdpPortsString()
    {
        return this->GetPortsString(false);
    }

private:
    QList<quint16> ParsePorts(QString input)
    {
        QList<quint16> output;

        QStringList ports = input.split(",");
        if( ports.count() > 0 )
        {
            for(const QString &port: ports)
            {
                if( this->IsValidPort(port) )
                {
                    output.append(port.toInt());
                }
            }
        }
        else if( !input.isEmpty() )
        {
            if( this->IsValidPort(input) )
            {
                output.append(input.toInt());
            }
        }

        return output;
    }

    bool IsValidPort(QString input)
    {
        bool isNum = false;
        int value = input.toInt(&isNum);
        if( isNum && value >= 1 && value <= 65535 )
            return true;
        return false;
    }

    QStringList ParseShodanDorks(QString input)
    {
        QStringList output;

        QStringList dorks = input.split(",");
        if(dorks.count() > 0 )
        {
            for( const QString &dork: dorks )
            {
                if( !dork.isEmpty() )
                {
                    output.append(dork);
                }
            }
        }
        else if( !input.isEmpty() )
        {
            output.append(input);
        }

        return output;
    }

    QString GetPortsString(bool tcp)
    {
        QString output = "";
        QList<quint16> *ports = (tcp ? &this->TcpPorts : &this->UdpPorts);
        for( const quint16 &port: *ports )
        {
            output += QString::number(port) + ",";
        }
        if( ports->count() > 0 )
        {
            output.chop(1);
        }

        return output;
    }

};

}} // Namespaces

#endif // _SERVICES_PORTSCANTARGETTYPE_H_
