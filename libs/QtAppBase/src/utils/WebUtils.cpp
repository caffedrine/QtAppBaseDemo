#include "WebUtils.h"

namespace utils { namespace web
{
    QString GetDomainNameFromUrl(QString url)
    {
        QStringList domainElements = url.split(".");
        if( domainElements.count() < 2 )
        {
            return "";
        }
        QString domain =  domainElements.at(domainElements.count() - 2) + "." + domainElements.last();
        if( domain.contains("//") )
        {
            domain = domain.split("//")[1];
        }

        if( domain.contains(":") )
        {
            return domain.split(":")[0];
        }
        else if( domain.contains("/") )
        {
            return domain.split("/")[0];
        }
        else if( domain.contains("?") )
        {
            return domain.split("?")[0];
        }

        return domain;
    }

}} // Namespaces