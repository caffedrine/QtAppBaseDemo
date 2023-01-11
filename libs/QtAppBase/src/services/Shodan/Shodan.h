#ifndef _SERVICES_SHODAN_H_
#define _SERVICES_SHODAN_H_

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkAccessManager>
#include "services/Web/HttpWebRequest.h"

class Shodan: public QObject
{
Q_OBJECT
public:
    const QString BASE_PATH = "https://api.shodan.io";

    explicit Shodan(QString apiKey);
    ~Shodan();

    QString GetAccountInfo()
    {
        QJsonDocument response = this->CallApi(QNetworkAccessManager::GetOperation, "/account/profile?key=" + this->ApiKey);
        if( response.object().empty() )
            return "error while connecting to shodan";

        if( response.object().contains("error"))
            return "shodan error:" + response.object().value("error").toString();

        return "Credits available: " + QString::number(response.object().value("credits").toInt(-1));
    }
    QString GetSearchFilters()
    {
        QJsonDocument response = this->CallApi(QNetworkAccessManager::GetOperation, "/shodan/host/search/filters?key=" + this->ApiKey);

        if( response.isArray() )
        {
            QString output = "";
            for( QJsonValue v: response.array() )
            {
                output += v.toString() + "\n";
            }
            return output;
        }

        if( response.isEmpty())
            return "error while connecting to shodan: empty json";

        if( response.object().contains("error"))
            return "shodan error:" + response.object().value("error").toString();

        return QString(response.toJson());
    }
    QString GetCountByQuery(QString query)
    {
        QJsonDocument response = this->CallApi(QNetworkAccessManager::GetOperation, "/shodan/host/count?key=" + this->ApiKey + "&query=" + query);

        if( response.object().isEmpty() )
            return "-1";

        if( response.object().contains("error") )
            return "-2";

        if( !response.object().contains("total") )
            return "-3";

        return QString::number( response.object().value("total").toInt(-4) );
    }
    QMap<QString, QJsonDocument> GetResultsByByQuery(QString query, int page = 1)
    {
        QJsonDocument response = this->CallApi(QNetworkAccessManager::GetOperation, "/shodan/host/search?key=" + this->ApiKey + "&page=" + QString::number(page) + "&query=" + query);

        if( response.object().isEmpty() )
        {
            return QMap<QString, QJsonDocument>({{"error", QJsonDocument::fromJson("{\"error\": \"empty obj\"}")}});
        }

        if( response.object().contains("error") )
        {
            return {{"error", QJsonDocument::fromJson("{\"error\": \"" + response.object().value("error").toString().toUtf8() + "\"}")}};
        }

        if( !response.object().contains("matches") )
        {
            return QMap<QString, QJsonDocument>({{"error", QJsonDocument::fromJson("{\"error\": \"no matches found in the response\"}")}});
        }

        QMap<QString, QJsonDocument> output;
        QJsonArray matches = response.object().value("matches").toArray();
        qDebug().nospace().noquote() << "Found " << matches.count()  << " matches for dork '" << query << "'";

        for( QJsonValueRef match: matches )
        {
            output[match.toObject().value("ip_str").toString()] = QJsonDocument(match.toObject());
        }

        return output;
    }

protected:
    QJsonDocument CallApi(QNetworkAccessManager::Operation method, const QString& endpoint);

private:
    HttpWebRequest *http = nullptr;
    QString ApiKey = "";

};

#endif // _SERVICES_SHODAN_H_
