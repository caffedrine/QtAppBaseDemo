#include "RawHttpRequestParser.h"

#include <QDebug>

namespace Services { namespace Parsers {

RawHttpRequestParser::RawHttpRequestParser()
{
    this->GlobalParserState = PARSE_FIRST_LINE;
}

RawHttpRequestParser::RawHttpRequestParser(QByteArray responseChunk)
{
    this->GlobalParserState = PARSE_FIRST_LINE;
    this->addData(responseChunk);
}

void RawHttpRequestParser::ParseFirstLine()
{
    if(!this->UnprocessedBodyDataReceived.contains("\r\n"))
    {
        this->ParseFailReason = "no first line with version detected";
        return;
    }

    // Extract first line and remove it from buffer
    QByteArray firstLine = this->UnprocessedBodyDataReceived.left(this->UnprocessedBodyDataReceived.indexOf("\r\n"));
    this->UnprocessedBodyDataReceived.remove(0, this->UnprocessedBodyDataReceived.indexOf("\r\n") + 2);

    // Three elements separated by three spaces
    if(!firstLine.contains(' '))
    {
        this->ParseFailReason = "first line does not contain properly formatted METHOD PATH VERSION (no spaces found)";
        this->GlobalParserState = PARSE_FAILED;
        return;
    }

    // First line must contain 3 elements
    QByteArrayList elements = firstLine.split(' ');
    if(elements.count() < 3)
    {
        this->ParseFailReason = "first line does not contain properly METHOD PATH VERSION (there must be 3 elements found)";
        this->GlobalParserState = PARSE_FAILED;
        return;
    }

    // First element must be HTTP Method
    this->Method = elements.first();

    // Last element must be version
    this->HttpVersion = elements.last();

    // Path is whatever is in the middle
    elements.pop_back();
    elements.pop_front();
    this->Path = elements.join(' ');

    // If all is good so far, advance to next state and parse headers
    this->GlobalParserState = PARSE_HEADERS;
    this->ParseFailReason.clear();
}

void RawHttpRequestParser::clear()
{
    this->Method.clear();;
    this->Path.clear();
    HttpParsersBase::clear();
}

QByteArray RawHttpRequestParser::GetFirstLine()
{
    return this->Method + " " + this->Path + " " + this->HttpVersion;
}

}} // Namespaces
