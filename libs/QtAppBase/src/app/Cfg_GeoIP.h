#ifndef _CFG_GEOIP_H_
#define _CFG_GEOIP_H_

#include <QtCore/QString>

namespace Services { namespace GeoIP { namespace Settings
{
    namespace GeoIPProviders
    {
        static const QString DbIP = "DB-IP";
        static const QString Maxmind = "MaxMind";
    }

    namespace DBIP
    {
        const QString IP2ASN_DB_PATH = "/data/GeoIP/DbIP/ip2asn.mmdb";
        const QString IP2COUNTRY_DB_PATH = "/data/GeoIP/DbIP/ip2country.mmdb";
    }

    namespace MAXMIND
    {
        const QString IP2ISP_DB_PATH = "/data/GeoIP/Maxmind/ip2isp.mmdb";
        const QString IP2ORG_DB_PATH = "/data/GeoIP/Maxmind/ip2org.mmdb";
    }
}}} // Namespaces

#endif // _CFG_GEOIP_H_
