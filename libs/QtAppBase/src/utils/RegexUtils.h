/*  
 *  File      : RegexUtils.h
 *  Created on: 2022-05-02
 *      Author: curiosul
 */

#ifndef _REGEXUTILS_H_
#define _REGEXUTILS_H_

#include <QString>
#include <QStringList>

namespace utils { namespace regex
{
    QStringList GetAllMatches(QString regexPattern, QString str);
}} // Namespaces

#endif // _REGEXUTILS_H_
