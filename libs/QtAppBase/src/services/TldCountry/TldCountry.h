#ifndef _SERVICES_TLDCOUNTRY_H_
#define _SERVICES_TLDCOUNTRY_H_

#include <QMap>

namespace Services { namespace TldCountry
{
    QString GetCountryByTLD(QString tld);
    QString GetTldByCountry(QString country_tld);
}} // Namespaces

#endif //_SERVICES_TLDCOUNTRY_H_
