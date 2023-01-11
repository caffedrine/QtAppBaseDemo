#include "RegexUtils.h"

#include <QRegularExpressionMatchIterator>

namespace utils { namespace regex
{
    QStringList GetAllMatches(QString regexPattern, QString str)
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
}} // Namespaces
