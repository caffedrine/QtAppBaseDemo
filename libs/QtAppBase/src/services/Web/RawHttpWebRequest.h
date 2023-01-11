#ifndef _RAWHTTPWEBREQUEST_H_
#define _RAWHTTPWEBREQUEST_H_

#include <QString>
#include <QByteArray>
#include <QSslSocket>
#include <QTimer>
#include "services/Web/Parsers/RawHttpResponseParser.h"
#include "services/Web/Parsers/RawHttpRequestParser.h"

namespace Services { namespace Web
{
    using Services::Parsers::RawHttpRequestParser;
    using Services::Parsers::RawHttpResponseParser;

    class RawHttpWebRequest: public QObject
    {
        enum state
        {
            WAIT_CONNECT,
            WAIT_WRITE,
            WAIT_READ
        };

        Q_OBJECT
    public:
        RawHttpWebRequest(QString host, quint16 port);
        ~RawHttpWebRequest();

        void SendHttp(const QByteArray &rawHttpRequest);
        void SendHttps(const QByteArray &rawHttpRequest);
        void Abort();

        void setConnectTimeoutMs(quint32 ms) { this->ConnectTimeout = ms;};
        void setReadTimeoutMs(quint32 ms) { this->ReadTimeout = ms; };
        void setMaxRetries(quint16 retries) { this->ConnectMaxRetries = retries; };

        QStringList GetLogsFlow();
    private:
        QString targetHost;
        quint16 targetPort;
        QString targetIp;
        QStringList flowLogs;

        quint32 ConnectTimeout = 10000, ReadTimeout = 5000;
        quint16 ConnectMaxRetries = 0;

        QTcpSocket *tcpSocket = nullptr;
        QTimer socketTimer;
        RawHttpRequestParser currRequest;
        RawHttpResponseParser currResponse;
        bool ErrorOccurred;

        void InitTcpSocket();
        void Log(const QString &str);

    signals:
        void RequestFinished(QTcpSocket *socket, const RawHttpRequestParser &request, const RawHttpResponseParser &response) const;
        void RequestReturnedError(QTcpSocket *socket, const RawHttpRequestParser &request, const QString &errorDescription, const RawHttpResponseParser &response) const;
        void RequestStateChangeInfo(QTcpSocket *socket, const RawHttpRequestParser &request, const RawHttpResponseParser &response, QString status);

    private slots:
        void Conn_HostFound();
        void Conn_SocketErrorOccurred(const QAbstractSocket::SocketError error);
        void Conn_SystemErrorOccurred(const QString errorDescription);
        void Conn_SslErrorOccurred(const QSslError error);
        void Conn_TcpConnected();
        void Conn_TcpDisconnected();
        void Conn_BytesWritten(quint64 bytes);
        void Conn_ReadyRead();

    };
}} // namespaces

#endif //_RAWHTTPWEBREQUEST_H_
