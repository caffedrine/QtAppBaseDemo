#include "DomainWhois.h"

#include <QTcpSocket>
#include <QRegularExpression>
#include <QEventLoop>
#include <QObject>
#include <QTimer>
#include <QHostAddress>

namespace Services { namespace Whois
{
    QString DomainWhois::GetDomainInfo(QString domain_name)
    {
        QString currServer = "whois.iana.org", currResponse = "";
        while( !currServer.isEmpty() )
        {
            currResponse = this->DownloadWhoisInfo(currServer, domain_name);
            currServer = this->ParseWhoisReponseAndGetForwardWhois(currServer, currResponse);
        }
        return currResponse;
    }

    QString DomainWhois::DownloadWhoisInfo(const QString &whoisServer, const QString &domainName)
    {
        const int TimeoutConnectMs = 2000;
        const int TimeoutWriteMs = 2000;
        const int TimeoutDataReadMs = 5000;
        const int TimeoutCloseSocket = 500;

        if( this->SHOW_DEBUG_MESSAGES ) qDebug().noquote().nospace() << "Start whois lookup for host " << domainName << " using server " << whoisServer;

        if( domainName.isEmpty() )
        {
            if( this->SHOW_DEBUG_MESSAGES ) qDebug().noquote().nospace() << "Empty domain name received";
            return "";
        }

        // Wait until response is received
        QEventLoop waitLoop;
        QTimer timer;
        QTcpSocket socket;

        QByteArray whoisResponse = "";
        bool ErrorDetected = false;
        bool HostDisconnected = true;
        QString errorDescription = "";

        typedef enum
        {
            WAIT_CONNECT = 0,
            WAIT_SEND_DATA = 1,
            WAIT_RECV_DATA = 2,
            WAIT_CLOSE = 3
        } step_t;
        step_t step = WAIT_CONNECT;

        // timeout handler
        QObject::connect(&timer, &QTimer::timeout, [&socket, &ErrorDetected, &errorDescription, &waitLoop, &step, &whoisServer, &whoisResponse, this]()
        {
            ErrorDetected = true;
            QString timeoutReason = "";
            if( step == WAIT_CONNECT)
                timeoutReason += "CONNECT";
            else if( step == WAIT_SEND_DATA )
                timeoutReason += "DATA_SEND";
            else if( step == WAIT_RECV_DATA )
                timeoutReason += "DATA_RECV";
            else if( step == WAIT_CLOSE )
                timeoutReason += "SOCKET_CLOSE";

            errorDescription = "\t -> [DOMAIN WHOIS] Timeout on socket " + QString::number(socket.socketDescriptor()) +
                    " while waiting for " + timeoutReason + " on host " + whoisServer + ":43";
            if( this->SHOW_DEBUG_MESSAGES ) qWarning().nospace().noquote() << errorDescription;

            waitLoop.quit();
        });

        // Socket error handler
        QObject::connect(&socket, &QTcpSocket::errorOccurred, [&timer, &waitLoop, &socket, &ErrorDetected, &errorDescription, this](QAbstractSocket::SocketError socketError)->void
        {
            timer.stop();
            ErrorDetected = true;

            errorDescription = "\t -> [DOMAIN WHOIS] Socket " + QString::number(socket.socketDescriptor()) + " error detected: " + socket.errorString();
            if( this->SHOW_DEBUG_MESSAGES ) qWarning().nospace().noquote() << errorDescription;
            waitLoop.quit();
        });

        // Socket connected handler
        QObject::connect(&socket, &QTcpSocket::connected, [&waitLoop, &socket, &HostDisconnected, this]()->void
        {
            HostDisconnected = false;
            if( this->SHOW_DEBUG_MESSAGES )qDebug().nospace().noquote() << "\t -> [DOMAIN WHOIS] Socket " << socket.socketDescriptor() << " connected to " << socket.peerAddress().toString() << ":" << socket.peerPort();
            waitLoop.quit();
        });

        // Socket disconnected handler
        QObject::connect(&socket, &QTcpSocket::disconnected, [&waitLoop, &socket, &HostDisconnected, this]()->void
        {
            HostDisconnected = true;
            if( this->SHOW_DEBUG_MESSAGES ) qDebug().nospace().noquote() << "\t -> [DOMAIN WHOIS] Disconnected from " << socket.peerAddress().toString() << ":" << socket.peerPort();
            waitLoop.quit();
        });

        // Data reception handler
        QObject::connect(&socket, &QTcpSocket::readyRead, [&waitLoop, &socket, &whoisResponse, this]()->void
        {
            // Disable error reporting after data was read
            QObject::disconnect(&socket, &QTcpSocket::errorOccurred, 0, 0);

            // Read data from socket and store it
            if( this->SHOW_DEBUG_MESSAGES ) qDebug().nospace().noquote() << "\t -> [DOMAIN WHOIS] Socket " << socket.socketDescriptor() << " data available: " << socket.bytesAvailable() << " bytes";
            whoisResponse += socket.readAll();

            // Do not break the loop as data might be fragmented and new packets might still arrive
            //waitLoop.quit();
        });

        // Data write finished handler
        QObject::connect(&socket, &QTcpSocket::bytesWritten, [&waitLoop, &socket, this](qint64 bytes)->void
        {
            if( this->SHOW_DEBUG_MESSAGES ) qDebug().nospace().noquote() << "\t -> [DOMAIN WHOIS] Written " << bytes << " bytes to socket " << socket.socketDescriptor() << ", " << socket.peerAddress().toString() << ":" << socket.peerPort();
            waitLoop.quit();
        });

        //  Connect to host and set timeout
        socket.connectToHost(whoisServer, 43);
        step = WAIT_CONNECT;
        if( !ErrorDetected )
        {
            timer.start(TimeoutConnectMs);
            waitLoop.exec();
            timer.stop();
        }

        // Write data
        if( !ErrorDetected )
        {
            step = WAIT_SEND_DATA;
            socket.write(QString(domainName + "\r\n").toUtf8()); // append cr lf at the end
            timer.start(TimeoutWriteMs);
            waitLoop.exec();
            timer.stop();
        }

        // Wait for data available
        if( !ErrorDetected )
        {
            step = WAIT_RECV_DATA;
            timer.start(TimeoutDataReadMs);
            waitLoop.exec();
            timer.stop();
        }

        // close socket if open
        if( !ErrorDetected )
        {
            step = WAIT_CLOSE;
            socket.close();
            if( !HostDisconnected )
            {
                timer.start(TimeoutCloseSocket);
                waitLoop.exec();
                timer.stop();
            }
        }
        else
        {
            socket.abort();
        }

        // Record the data and return whois response
        this->WhoisServers.push_back(whoisServer);
        this->ServersResponses.push_back(whoisResponse + (!errorDescription.isEmpty() ? "\n" + errorDescription.trimmed() : ""));
        return whoisResponse;
    }

    QString DomainWhois::ParseWhoisReponseAndGetForwardWhois(const QString &whoisServer, const QString &whoisReponse)
    {
        if( whoisReponse.isEmpty() )
            return "";

        QRegularExpression re("whois\\.[a-zA-Z0-9][a-zA-Z0-9-]{1,61}[a-zA-Z0-9](?:\\.[a-zA-Z]{2,})");

        // Check if response contains a new whois server following pattern whois.domain.tld
        auto matches = re.globalMatch(whoisReponse);
        if( matches.hasNext() )
        {
            QString match = matches.next().captured();
            if( match.trimmed().toLower() != whoisServer.toLower() )
            {
                return match.trimmed();
            }
        }
        return "";
    }
}}; // Namespaces
