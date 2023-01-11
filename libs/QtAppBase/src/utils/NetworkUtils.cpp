#include "NetworkUtils.h"

#include <QHostAddress>
#include <QAbstractSocket>
#include <QRegularExpressionMatchIterator>

namespace utils { namespace network
{
    QStringList GetRegexAllMatches(QString regexPattern, QString str)
    {
        QStringList list;

        /* Regex to match ip address */
        QRegularExpression validator(regexPattern);

        /* Get matches */
        QRegularExpressionMatchIterator lineMatches = validator.globalMatch(str);

        /* Fill list with matches*/
        while(lineMatches.hasNext())
        {
            QRegularExpressionMatch match = lineMatches.next();
            if(match.hasMatch())
            {
                list.append(match.captured(0));
            }
        }

        return list;
    }

    namespace ipv4
    {
        bool IsValidIP(QString ip)
        {
            QHostAddress address(ip);
            if(QAbstractSocket::IPv4Protocol == address.protocol())
            {
                return true;
            }

            return false;
        }

        QStringList ExtractIpAddresses(QString str)
        {
            /* Regex to match ip address */
            QString regexPattern = ("((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)");

            return GetRegexAllMatches(regexPattern, str);
        }

        QStringList ExtractIpPortProxies(QString input)
        {
            /* Proxy pattern */
            QString regexPattern = "(((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\:((6553[0-5])|(655[0-2][0-9])|(65[0-4][0-9]{2})|(6[0-4][0-9]{3})|([1-5][0-9]{4})|([0-5]{1,5})|([0-9]{1,4})))";

            /* Fetch proxies from string */
            return GetRegexAllMatches(regexPattern, input);
        }

        quint32 Dotted2Long(QString ip)
        {
            QHostAddress address(ip);
            return address.toIPv4Address();
        }

        QString Dec2Dotted(quint32 ip)
        {
            QHostAddress address(ip);
            return address.toString();
        }

        void SortIpAddressesAsc(QStringList &input_list)
        {

        }
    }

    namespace ipv6
    {
        bool IsValidIP(QString ip)
        {
            QHostAddress address(ip);
            if(QAbstractSocket::IPv6Protocol == address.protocol())
            {
                /* IP given is v6 */
                return true;
            }
            return false;
        }

        QStringList ExtractIpAddresses(QString str)
        {
            /* Regex to match ip address */
            QString regexPattern = ("(([0-9a-fA-F]{1,4}:){7,7}[0-9a-fA-F]{1,4}|([0-9a-fA-F]{1,4}:){1,7}:|([0-9a-fA-F]{1,4}:){1,6}:[0-9a-fA-F]{1,4}|([0-9a-fA-F]{1,4}:){1,5}(:[0-9a-fA-F]{1,4}){1,2}|([0-9a-fA-F]{1,4}:){1,4}(:[0-9a-fA-F]{1,4}){1,3}|([0-9a-fA-F]{1,4}:){1,3}(:[0-9a-fA-F]{1,4}){1,4}|([0-9a-fA-F]{1,4}:){1,2}(:[0-9a-fA-F]{1,4}){1,5}|[0-9a-fA-F]{1,4}:((:[0-9a-fA-F]{1,4}){1,6})|:((:[0-9a-fA-F]{1,4}){1,7}|:)|fe80:(:[0-9a-fA-F]{0,4}){0,4}%[0-9a-zA-Z]{1,}|::(ffff(:0{1,4}){0,1}:){0,1}((25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])\\.){3,3}(25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])|([0-9a-fA-F]{1,4}:){1,4}:((25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])\\.){3,3}(25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9]))");

            return GetRegexAllMatches(regexPattern, str);
        }

        QStringList ExtractIpPortProxies(QString input)
        {
            /* Proxy pattern */
            QString regexPattern = "((([0-9a-fA-F]{1,4}:){7,7}[0-9a-fA-F]{1,4}|([0-9a-fA-F]{1,4}:){1,7}:|([0-9a-fA-F]{1,4}:){1,6}:[0-9a-fA-F]{1,4}|([0-9a-fA-F]{1,4}:){1,5}(:[0-9a-fA-F]{1,4}){1,2}|([0-9a-fA-F]{1,4}:){1,4}(:[0-9a-fA-F]{1,4}){1,3}|([0-9a-fA-F]{1,4}:){1,3}(:[0-9a-fA-F]{1,4}){1,4}|([0-9a-fA-F]{1,4}:){1,2}(:[0-9a-fA-F]{1,4}){1,5}|[0-9a-fA-F]{1,4}:((:[0-9a-fA-F]{1,4}){1,6})|:((:[0-9a-fA-F]{1,4}){1,7}|:)|fe80:(:[0-9a-fA-F]{0,4}){0,4}%[0-9a-zA-Z]{1,}|::(ffff(:0{1,4}){0,1}:){0,1}((25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])\\.){3,3}(25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])|([0-9a-fA-F]{1,4}:){1,4}:((25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])\\.){3,3}(25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9]))\\:((6553[0-5])|(655[0-2][0-9])|(65[0-4][0-9]{2})|(6[0-4][0-9]{3})|([1-5][0-9]{4})|([0-5]{1,5})|([0-9]{1,4})))";

            /* Fetch proxies from string */
            return GetRegexAllMatches(regexPattern, input);
        }
    }
}} // Namespaces
