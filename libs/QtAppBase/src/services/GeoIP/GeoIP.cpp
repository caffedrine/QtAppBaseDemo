#include "GeoIP.h"

#include <QLocale>
#include <QMutexLocker>

namespace Services { namespace GeoIP
{

// Class instance init
GeoIP *GeoIP::instance = nullptr;

GeoIP::GeoIP()
{
}

GeoIP::~GeoIP()
{
    delete this->dbip;
    this->dbip = nullptr;

    delete this->maxmind;
    this->maxmind = nullptr;
}

GeoIP *GeoIP::Instance()
{
    if (GeoIP::instance == nullptr)
    {
        GeoIP::instance = new GeoIP();
    }
    return GeoIP::instance;
}

void GeoIP::DestroyInstance()
{
    delete GeoIP::instance;
    GeoIP::instance = nullptr;
}

QString GeoIP::IP2CountryISO(const QString &ip_address)
{
    QMutexLocker ml(&this->mMutex);
    if (this->dbip == nullptr)
    {
        this->dbip = new DbIP();
    }
    return this->dbip->IP2CountryISO(QCoreApplication::applicationDirPath() + Settings::DBIP::IP2COUNTRY_DB_PATH, ip_address);
}

QString GeoIP::IP2CountryName(const QString &ip_address)
{
    QMutexLocker ml(&this->mMutex);
    if (this->dbip == nullptr)
    {
        this->dbip = new DbIP();
    }
    return this->dbip->IP2CountryName(QCoreApplication::applicationDirPath() + Settings::DBIP::IP2COUNTRY_DB_PATH, ip_address);
}

QString GeoIP::IP2Asn(const QString &ip_address)
{
    QMutexLocker ml(&this->mMutex);
    if (this->dbip == nullptr)
    {
        this->dbip = new DbIP();
    }
    return this->dbip->IP2Asn(QCoreApplication::applicationDirPath() + Settings::DBIP::IP2ASN_DB_PATH, ip_address);
}

QString GeoIP::IP2Org(const QString &ip_address)
{
    QMutexLocker ml(&this->mMutex);
    if (this->dbip == nullptr)
    {
        this->dbip = new DbIP();
    }
    return this->dbip->IP2Org(QCoreApplication::applicationDirPath() + Settings::DBIP::IP2ASN_DB_PATH, ip_address);
}

}} // Namespaces
