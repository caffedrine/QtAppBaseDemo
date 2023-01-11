#include <QEventLoop>
#include <QCoreApplication>
#include <QFileInfo>
#include "Shodan.h"

#include "services/Web/HttpWebRequest.h"

Shodan::Shodan(QString apiKey): ApiKey(apiKey)
{
    QMap<QByteArray, QByteArray> headers;
    headers["User-Agent"] = QFileInfo(QCoreApplication::applicationFilePath()).completeBaseName().toUtf8();
    headers["Content-type"] = "application/json";

    this->http = new HttpWebRequest();
    this->http->setHeaders(headers);
    this->http->setTimeout(10000);
}

Shodan::~Shodan()
{
    delete this->http;
    this->http = nullptr;
}

QJsonDocument Shodan::CallApi(QNetworkAccessManager::Operation method, const QString& endpoint)
{
    QJsonDocument output;

    // Wait until response is received
    QEventLoop waitLoop;

    QObject::connect(this->http, &HttpWebRequest::RequestStarted, this, [this](const QString &requestMethod, const QNetworkRequest *request, const QByteArray &requestBody)// clazy:exclude=lambda-in-connect
    {
        QByteArray requestHeaders;
        for(const QByteArray &header: request->rawHeaderList())
        {
            requestHeaders += header + ": " + request->rawHeader(header) + "\n";
        }
        if(requestHeaders.size() > 0 )
            requestHeaders.chop(1);

        //qDebug().noquote().nospace() << "SHODAN REQUEST:\n" << requestMethod << " " << request->url().toString() << "\n" << requestHeaders << (requestBody.count() ? "\n\n" + requestBody.trimmed() : "");
        qDebug().noquote().nospace() << "SHODAN REQUEST: " << requestMethod << " " << request->url().toString().replace(this->ApiKey, "xxx");
    });
    QObject::connect(this->http, &HttpWebRequest::RequestFinished, this, [&waitLoop, &output](const HttpWebRequestsResponse *response)// clazy:exclude=lambda-in-connect
    {
        QJsonParseError json_parse_error;
        //qDebug().noquote().nospace() << "RESPONSE:\n" << response->GetResponseHeaders() << (response->responseBody.count() ? "\n\n" + response->responseBody.trimmed(): "");
        //qDebug().noquote().nospace() << "SHODAN RESPONSE: " << response->HttpCode << " " << (response->responseBody.count() ? response->responseBody.trimmed(): "");
        qDebug().noquote().nospace() << "SHODAN RESPONSE: " << response->HttpCode;
        output = QJsonDocument::fromJson(response->responseBody.trimmed(), &json_parse_error);
        if( json_parse_error.error != QJsonParseError::NoError )
        {
            qDebug().noquote().nospace() << "JSON parsing error: " << json_parse_error.errorString();
        }
        waitLoop.quit();
    });
    QObject::connect(this->http, &HttpWebRequest::RequestReturnedError, this, [&waitLoop, &output, this](const HttpWebRequestsResponse *response)// clazy:exclude=lambda-in-connect
    {
        QJsonParseError json_parse_error;
        QByteArray jsonError = QString("{\"error\": \"" + response->reply->errorString().trimmed().replace(this->ApiKey, "xxx") + "\"}").toUtf8();
        //qDebug().noquote().nospace() << "ERROR: " << response->reply->errorString() << "\n" << response->GetResponseHeaders().trimmed() << (response->responseBody.count() ? "\n\n" + response->responseBody.trimmed() : "");
        qDebug().noquote().nospace() << "SHODAN ERROR: " << response->HttpCode << " " << response->reply->errorString().replace(this->ApiKey, "xxx") << "\n" << (response->responseBody.size() ? response->responseBody.trimmed() : "");
        output = QJsonDocument::fromJson(jsonError, &json_parse_error);
        if( json_parse_error.error != QJsonParseError::NoError )
        {
            qDebug().noquote().nospace() << "JSON parsing error: " << json_parse_error.errorString();
        }
        waitLoop.quit();
    });

    if( method == QNetworkAccessManager::Operation::GetOperation)
    {
        http->GET(this->BASE_PATH + endpoint);
    }
    else
    {
        qDebug() << "Wrong method!";
        return QJsonDocument();
    }

    waitLoop.exec();

    // Remove connections as these will persist
    QObject::disconnect(this->http, &HttpWebRequest::RequestStarted, this, nullptr);
    QObject::disconnect(this->http, &HttpWebRequest::RequestFinished, this, nullptr);
    QObject::disconnect(this->http, &HttpWebRequest::RequestReturnedError, this, nullptr);

    return output;
}
