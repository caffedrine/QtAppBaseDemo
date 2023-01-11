#ifndef _RAWHTTPREQUESTPARSER_H_
#define _RAWHTTPREQUESTPARSER_H_

#include <QByteArray>
#include <QMap>
#include "utils/utils.h"
#include "HttpParsersBase.h"

namespace Services { namespace Parsers {

class RawHttpRequestParser: public Services::Parsers::HttpParsersBase
{
public:
    RawHttpRequestParser();
    RawHttpRequestParser(QByteArray responseChunk);

    QByteArray GetMethod() { return this->Method; };
    QByteArray GetPath() { return this->Path; };
    QByteArray GetFirstLine();

    void clear();

protected:
    QByteArray Method;
    QByteArray Path;
    void ParseFirstLine();
};

}} // Namespaces

#endif //_RAWHTTPREQUESTPARSER_H_
