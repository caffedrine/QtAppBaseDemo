//
// https://gist.github.com/olafurjohannsson/801e0bd1428aec51bced75907c58c551
//

#ifndef _SERVICES_HTTPWEBREQUEST_H_
#define _SERVICES_HTTPWEBREQUEST_H_

#include <QObject>
#include <QString>
#include <QByteArray>
#include <QMap>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include "Compiler.h"
#include "HttpStatusCodes.h"

class HttpWebRequestsResponse
{
public:
    bool HttpErrorDetected = false;
    QString HttpErrorDescription = "";

    bool NetworkErrorDetected = false;
    QString NetworkErrorDescription = "";

    bool AppErrorDetected = false;
    QString AppErrorDesc = "";

    quint32 HttpCode;
    QString HttpCodeDesc;

    bool Redirected = false;
    QNetworkReply *reply;
    QByteArray responseBody;
    
    CORE_NODISCARD QString toString() const
    {
        QString result;

        result += (AppErrorDetected||NetworkErrorDetected||HttpErrorDetected)?"[FAILED] ":"[SUCCEED]";
        result += AppErrorDetected ? "[APPL ERR: " +  AppErrorDesc + "] " : "";
        result += NetworkErrorDetected ? NetworkErrorDescription + " " : "";
        result += HttpErrorDetected ? "[HTTP ERR: " +  HttpErrorDescription + "] " : "";
        result +=  !(AppErrorDetected||NetworkErrorDetected||HttpErrorDetected)?"[" + QString::number(this->HttpCode) + "] ":"";
        result += this->HttpMethod() + " ";
        result += this->reply->url().toString();
       // result += this->reply->readAll().count() > 0 ? "\n" + QString(this->reply->readAll()) + "\n" : "";

       return result;
    }
    
    CORE_NODISCARD QString HttpMethod() const
    {
        QMap<QNetworkAccessManager::Operation, QString> opStrings;
        opStrings[QNetworkAccessManager::Operation::UnknownOperation]   = "UNKNOWN";
        opStrings[QNetworkAccessManager::Operation::HeadOperation]      = "HEAD";
        opStrings[QNetworkAccessManager::Operation::GetOperation]       = "GET";
        opStrings[QNetworkAccessManager::Operation::PutOperation]       = "PUT";
        opStrings[QNetworkAccessManager::Operation::PostOperation]      = "POST";
        opStrings[QNetworkAccessManager::Operation::DeleteOperation]    = "DELETE";
        opStrings[QNetworkAccessManager::Operation::CustomOperation]    = "CUSTOM";
        return opStrings[reply->operation()];
    }
    
    CORE_NODISCARD QByteArray GetResponseHeaders() const
    {
        QByteArray output;
        for(const QNetworkReply::RawHeaderPair &pair: this->reply->rawHeaderPairs())
        {
            output += pair.first + ": " + pair.second + "\n";
        }
        if( output.size() > 0 )
            output.chop(1);

        return output;
    }
    
    CORE_NODISCARD QString ErrorString() const
    {
        QString result = "";
        result += AppErrorDetected ? "Appl error: " +  AppErrorDesc : "";
        result += NetworkErrorDetected ? "Network error: " + NetworkErrorDescription : "";
        result += HttpErrorDetected ? "Http error: " +  HttpErrorDescription : "";
        return result;
    }
};

class HttpWebRequest: public QObject
{
    Q_OBJECT
public:
    explicit HttpWebRequest(QObject *parent = nullptr);
    ~HttpWebRequest() override;
    void setHeaders(const QMap<QByteArray, QByteArray> &headers);
    void setTimeout(int ms);
    void setIgnoreSslErrors(bool enabled);

    void GET(const QString& url);
    void POST(const QString& url, const QByteArray &rawData);
    void POST(const QString& url, const QMap<QString, QString> &data);
    void CUSTOM(QByteArray method, const QString &url, const QByteArray &data);
    void PUT(const QString& url, const QByteArray &rawData);
    void PUT(const QString& url, const QMap<QString, QString> &data);
    void HEAD(const QString& url);
    void DELETE(const QString& url);


signals:
    void RequestStarted(const QString &requestMethod, const QNetworkRequest *request, const QByteArray &requestBody) const;
    void RequestFinished(const HttpWebRequestsResponse *response) const;
    void RequestReturnedError(const HttpWebRequestsResponse *response) const;

protected slots:
    void onNetworkReplyFinished(QNetworkReply *networkReply) const;

private:
    QNetworkAccessManager *networkManager;
    QMap<QByteArray, QByteArray> headers;
    bool IgnoreSslErrors = false;

    QUrlQuery constructNetworkRequestQuery(const QMap<QString, QString> &data);
    QNetworkRequest constructNetworkRequest(const QString& hostName);
    void ApplyReplySettings(QNetworkReply *reply);
};


Q_DECLARE_METATYPE(HttpWebRequestsResponse)

#endif // _SERVICES_HTTPWEBREQUEST_H_
