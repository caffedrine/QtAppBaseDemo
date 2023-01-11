#ifndef _HTTPPARSERSBASE_H_
#define _HTTPPARSERSBASE_H_

#include <QByteArray>
#include <QString>
#include <QList>
#include <QMap>

#include "utils/utils.h"

namespace Services { namespace Parsers {

    class HttpParsersBase
    {
        enum ChunksParserState
        {
            CHNK_INVALID = 0,
            CHNK_WAIT_LENGTH = 1,
            CHNK_WAIT_CONTENT = 2,
            CHNK_WAIT_CONTENT_EOF = 3,
            CHNK_WAIT_FINAL_EOF = 4,
            CHNK_PARSE_COMPLETED = 5
        };

        ChunksParserState ChunkedParsingState = CHNK_INVALID;
        quint64 currentChunkLength = -1;

    public:
        void addData(QByteArray chunkData);

        bool HaveHeader(QByteArray headerName);
        QByteArray GetHeaderByName(QByteArray headerName);
        QByteArray GetRaw();
        QByteArray GetRawHeaders();
        QByteArray GetRawBody();
        QByteArray GetHttpVersion() { return this->HttpVersion; };
        QString GetParseFailReason();

        bool IsValidHttpData();
        bool IsCompleteHttpData();

    protected:
        HttpParsersBase() = default;

        enum GlobalParserState
        {
            PARSE_FIRST_LINE,
            PARSE_HEADERS,
            PARSE_BODY,
            PARSE_COMPLETED,
            PARSE_FAILED,
        };

        QByteArray UnprocessedBodyDataReceived;
        QByteArray RawData;
        GlobalParserState GlobalParserState = PARSE_FIRST_LINE;
        QString ParseFailReason;

        QByteArray HttpVersion;
        QList<QByteArray> HeadersNames;
        QList<QByteArray> HeadersVals;
        QMap<QByteArray, int>HeaderIdx; // Can't use single Map due to duplicate headers
        QByteArray Body;
        bool UnspecifiedBodyLength = false;

        virtual QByteArray GetFirstLine() = 0;
        virtual void clear();

        virtual void ParseFirstLine() = 0;
        virtual void ParseHeaders();
        virtual void ParseBody();

        // HTTP fragments - still parts of the body
        virtual void ParseContentLengthBody();
        virtual void ParseChunkedTransferEncoding();
    };
}}; // Namespaces

#endif //_HTTPPARSERSBASE_H_
