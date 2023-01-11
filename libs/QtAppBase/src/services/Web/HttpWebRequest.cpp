#include "HttpWebRequest.h"

#include <utility>
#include <QUrlQuery>

HttpWebRequest::HttpWebRequest(QObject *parent): QObject(parent)
{
    // create network manager
    this->networkManager = new QNetworkAccessManager(this);
    this->networkManager->setAutoDeleteReplies(true);
    connect(this->networkManager, SIGNAL(finished(QNetworkReply * )), this, SLOT(onNetworkReplyFinished(QNetworkReply * )));
}

HttpWebRequest::~HttpWebRequest()
{
    delete this->networkManager;
}

void HttpWebRequest::HEAD(const QString &url)
{
    QNetworkRequest request = this->constructNetworkRequest(url);
    QNetworkReply *reply = this->networkManager->head(request);
    this->ApplyReplySettings(reply);
    emit this->RequestStarted("HEAD", &request, QByteArray());
}

void HttpWebRequest::GET(const QString &url)
{
    QNetworkRequest request = this->constructNetworkRequest(url);
    QNetworkReply *reply = this->networkManager->get(request);
    this->ApplyReplySettings(reply);
    emit this->RequestStarted("GET", &request, QByteArray());
}

void HttpWebRequest::DELETE(const QString &url)
{
    QNetworkRequest request = this->constructNetworkRequest(url);
    QNetworkReply *reply = this->networkManager->deleteResource(request);
    this->ApplyReplySettings(reply);
    emit this->RequestStarted("DELETE", &request, QByteArray());
}

void HttpWebRequest::PUT(const QString &url, const QByteArray &rawData)
{
    QNetworkRequest request = this->constructNetworkRequest(url);
    QNetworkReply *reply = this->networkManager->put(request, rawData);
    this->ApplyReplySettings(reply);
    emit this->RequestStarted("PUT", &request, rawData);
}

void HttpWebRequest::PUT(const QString &url, const QMap<QString, QString> &data)
{
    QNetworkRequest request = this->constructNetworkRequest(url);
    QByteArray putData = this->constructNetworkRequestQuery(data).toString(QUrl::FullyEncoded).toUtf8();
    QNetworkReply *reply = this->networkManager->put(request, putData);
    this->ApplyReplySettings(reply);
    emit this->RequestStarted("PUT", &request, putData);
}

void HttpWebRequest::POST(const QString &url, const QByteArray &data)
{
    QNetworkRequest request = this->constructNetworkRequest(url);
    QNetworkReply *reply = this->networkManager->post(request, data);
    this->ApplyReplySettings(reply);
    emit this->RequestStarted("POST", &request, data);
}

void HttpWebRequest::POST(const QString &url, const QMap<QString, QString> &data)
{
    QNetworkRequest request = this->constructNetworkRequest(url);
    QByteArray postData = this->constructNetworkRequestQuery(data).toString(QUrl::FullyEncoded).toUtf8();
    QNetworkReply *reply = this->networkManager->post(request, postData);
    this->ApplyReplySettings(reply);
    emit this->RequestStarted("POST", &request, postData);
}

void HttpWebRequest::CUSTOM(QByteArray method, const QString &url, const QByteArray &data)
{
    QNetworkRequest request = this->constructNetworkRequest(url);
    QNetworkReply *reply = this->networkManager->sendCustomRequest(request, method, data);
    this->ApplyReplySettings(reply);
    emit this->RequestStarted(method, &request, data);
}

void HttpWebRequest::onNetworkReplyFinished(QNetworkReply *networkReply) const
{
    // build response
    HttpWebRequestsResponse response;
    response.reply = networkReply;
    response.responseBody = networkReply->readAll();

    // no error in request
    if(networkReply->attribute( QNetworkRequest::HttpStatusCodeAttribute ).isValid())
    {
        // get HTTP status code
        qint32 httpStatusCode = networkReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        response.HttpCode = httpStatusCode;
        response.HttpCodeDesc = networkReply->attribute( QNetworkRequest::HttpReasonPhraseAttribute ).toString();
        if( response.HttpCodeDesc.isEmpty() )
            response.HttpCodeDesc = HttpStatus::reasonPhrase((int)response.HttpCode);

        // 200
        if(httpStatusCode >= 200 && httpStatusCode < 300) // OK
        {
            this->RequestFinished(&response);
        }
        else if(httpStatusCode >= 300 && httpStatusCode < 400) // 300 Redirect
        {
            response.Redirected = true;

            // Get new url, can be relative
            QUrl relativeUrl = networkReply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();

            // url can be relative, we use the previous url to resolve it
            QUrl redirectUrl = networkReply->url().resolved(relativeUrl);

            // redirect to new url
            networkReply->manager()->get(QNetworkRequest(redirectUrl));

            // maintain manager
            return;
        }
        else if(httpStatusCode >= 400 && httpStatusCode <= 600) // 400 Error
        {
            response.HttpErrorDetected = true;
            response.HttpErrorDescription = QString::number(httpStatusCode) + " " + response.HttpCodeDesc;
            emit this->RequestReturnedError(&response);
        }
        else
        {
            response.HttpErrorDetected = true;
            response.HttpErrorDescription = "Invalid response code " + QString::number(httpStatusCode);
            emit this->RequestReturnedError(&response);
        }
    }
    else
    {
        response.NetworkErrorDetected = true;
        response.NetworkErrorDescription = "[" + QString::number(networkReply->error()) + "] " + networkReply->errorString();
        emit this->RequestReturnedError(&response);
    }

    networkReply->close();
    networkReply->deleteLater();
}

/*
 *
 *  HELPERS
 *
 */

QUrlQuery HttpWebRequest::constructNetworkRequestQuery(const QMap<QString, QString> &data)
{
    // Create POST/PUT data
    QUrlQuery postData;
    QMapIterator<QString, QString> iterator(data);

    // add all keys from map
    while(iterator.hasNext())
    {
        iterator.next();
        postData.addQueryItem(iterator.key(), iterator.value());
    }
    return postData;
}

QNetworkRequest HttpWebRequest::constructNetworkRequest(const QString &hostName)
{
    // create HTTP request and set hostname
    QNetworkRequest request;
    request.setUrl(QUrl(hostName));

    // add headers
    if(!headers.isEmpty())
    {
        QMapIterator<QByteArray, QByteArray> iterator(headers);
        while(iterator.hasNext())
        {
            iterator.next();
            request.setRawHeader(QByteArray::fromStdString(iterator.key().toStdString()), QByteArray::fromStdString(iterator.value().toStdString()));
        }
    }

    return request;
}

void HttpWebRequest::setHeaders(const QMap<QByteArray, QByteArray> &headers_)
{
    this->headers = headers_;
}

void HttpWebRequest::setTimeout(int ms)
{
    this->networkManager->setTransferTimeout(ms);
}

void HttpWebRequest::ApplyReplySettings(QNetworkReply *reply)
{
    if( this->IgnoreSslErrors )
    {
        reply->ignoreSslErrors();
    }
}

void HttpWebRequest::setIgnoreSslErrors(bool enabled)
{
    this->IgnoreSslErrors = enabled;
}


