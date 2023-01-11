#include "RawHttpResponseParser.h"

#include <QDebug>

namespace Services { namespace Parsers {

    RawHttpResponseParser::RawHttpResponseParser()
    {
        this->GlobalParserState = PARSE_FIRST_LINE;
    }

    RawHttpResponseParser::RawHttpResponseParser(QByteArray responseChunk)
    {
        this->GlobalParserState = PARSE_FIRST_LINE;
        this->addData(responseChunk);
    }

    bool RawHttpResponseParser::IsKeepAliveEnabled()
    {
        return false;
    }

    void RawHttpResponseParser::ParseFirstLine()
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
            this->ParseFailReason = "first line does not contain properly formatted version and response code (no spaces found)";
            this->GlobalParserState = PARSE_FAILED;
            return;
        }

        // First line must contain 3 elements
        QByteArrayList elements = firstLine.split(' ');
        if(elements.length() < 2)
        {
            this->ParseFailReason = "first line does not contain properly formatted version and response code (there must be 2-3 elements found)";
            this->GlobalParserState = PARSE_FAILED;
            return;
        }

        // First element must be HTTP version
        if(!elements.first().toLower().startsWith("http/"))
        {
            this->ParseFailReason = "invalid http version";
            this->GlobalParserState = PARSE_FAILED;
            return;
        }
        this->HttpVersion = elements.first();
        elements.pop_front();

        bool isNumber = false;
        int intNo = QString(elements.front()).toInt(&isNumber);
        if(!isNumber)
        {
            this->ParseFailReason = "invalid response code";
            this->GlobalParserState = PARSE_FAILED;
            return;
        }
        this->ResponseCode = intNo;
        elements.pop_front();

        // Whatever is remaining, is the code description
        if( !elements.empty() )
            this->ResponseCodeDesc = elements.join(' ');

        // If all is good so far, advance to next state and parse headers
        this->GlobalParserState = PARSE_HEADERS;
        this->ParseFailReason.clear();
    }

    void RawHttpResponseParser::clear()
    {
        this->ResponseCode = -1;
        this->ResponseCodeDesc.clear();
        HttpParsersBase::clear();
    }

    QByteArray RawHttpResponseParser::GetFirstLine()
    {
        return this->HttpVersion + " " + (this->ResponseCode > 0 ? QString::number(this->ResponseCode).toUtf8() : " ") + " " + this->ResponseCodeDesc;
    }

    void RawHttpResponseParser::ConnectionCloseReceived()
    {
        // If body length cannot be determined and client added "Connection: close" header, then we consider body complete whenever server closes the connection
        if(this->UnspecifiedBodyLength)
        {
            this->GlobalParserState = GlobalParserState::PARSE_COMPLETED;
        }
    }
}} // Namespaces
