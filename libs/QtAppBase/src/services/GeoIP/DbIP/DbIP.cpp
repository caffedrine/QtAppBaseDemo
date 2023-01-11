#include "DbIP.h"

#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QHostAddress>
#include <QVariant>

namespace Services { namespace GeoIP
{

DbIP::DbIP()
{

}

DbIP::~DbIP()
{
    delete this->CountryDbReader;
    this->CountryDbReader = nullptr;

    delete this->AsnDbReader;
    this->AsnDbReader = nullptr;
}

QString DbIP::IP2CountryISO(const QString &CountryDbPath, const QString &ip_address)
{
    QString error;
    if(this->CountryDbReader == nullptr )
    {
        this->CountryDbReader = MmdbReader::load(CountryDbPath, error);
        if(this->CountryDbReader == nullptr)
        {
            qDebug() << "Cannot load DB-IP ip2country database ("<<CountryDbPath<<"): "<< error;
            return "";
        }
    }

    QVariant lookupResultBlock = this->CountryDbReader->lookup(QHostAddress(ip_address));
    return lookupResultBlock.toHash()["country"].toHash()["iso_code"].toString();
}

QString DbIP::IP2CountryName(const QString &CountryDbPath, const QString &ip_address)
{
    QString error;
    if(this->CountryDbReader == nullptr )
    {
        this->CountryDbReader = MmdbReader::load(CountryDbPath, error);
        if(this->CountryDbReader == nullptr)
        {
            qDebug() << "Cannot load DB-IP ip2country database ("<<CountryDbPath<<"): "<< error;
            return "";
        }
    }

    QVariant lookupResultBlock = this->CountryDbReader->lookup(QHostAddress(ip_address));
    return lookupResultBlock.toHash()["country"].toHash()["names"].toHash()["en"].toString();
}

QString DbIP::IP2Asn(const QString &AsnDbPath, const QString &ip_address)
{
    QString error;
    if(this->AsnDbReader == nullptr )
    {
        this->AsnDbReader = MmdbReader::load(AsnDbPath, error);
        if(this->AsnDbReader == nullptr)
        {
            qDebug() << "Cannot load DB-IP ip2asn database: "<< error;
            return "";
        }
    }

    QVariant lookupResultBlock = this->AsnDbReader->lookup(QHostAddress(ip_address));
    return "ASN" + lookupResultBlock.toHash()["autonomous_system_number"].toString();
}

QString DbIP::IP2Org(const QString &AsnDbPath, const QString &ip_address)
{
    QString error;
    if(this->AsnDbReader == nullptr )
    {
        this->AsnDbReader = MmdbReader::load(AsnDbPath, error);
        if(this->AsnDbReader == nullptr)
        {
            qDebug() << "Cannot load DB-IP ip2asn database: "<< error;
            return "";
        }
    }

    QVariant lookupResultBlock = this->AsnDbReader->lookup(QHostAddress(ip_address));
    return lookupResultBlock.toHash()["autonomous_system_organization"].toString();
}

}} // Namespaces
