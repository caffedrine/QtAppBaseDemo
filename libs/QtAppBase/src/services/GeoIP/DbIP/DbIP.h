#ifndef _DBIP_H_
#define _DBIP_H_

#include <QtCore/QString>
#include <QtCore/QDebug>

#include "services/GeoIP/MmdbReader.h"

namespace Services { namespace GeoIP
{

class DbIP
{
public:
    static const int MAX_DB_SIZE = 64 * 1024 * 1024;
    DbIP();
    ~DbIP();

    QString IP2CountryISO(const QString &CountryDbPath, const QString &ip_address);
    QString IP2CountryName(const QString &CountryDbPath, const QString &ip_address);
    QString IP2Asn(const QString &AsnDbPath, const QString &ip_address);
    QString IP2Org(const QString &AsnDbPath, const QString &ip_address);

protected:


private:
    MmdbReader *CountryDbReader = nullptr;
    MmdbReader *AsnDbReader = nullptr;

};

}} // Namespaces

#endif // _DBIP_H_
