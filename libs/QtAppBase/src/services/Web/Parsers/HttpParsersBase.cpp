#include "HttpParsersBase.h"

namespace Services { namespace Parsers {

    void HttpParsersBase::addData(QByteArray chunkData)
    {
        this->UnprocessedBodyDataReceived.append(chunkData);
        this->RawData.append(chunkData);

        if(this->GlobalParserState == PARSE_FIRST_LINE)
        {
            this->ParseFirstLine();
        }

        if(this->GlobalParserState == PARSE_HEADERS)
        {
            this->ParseHeaders();
        }

        if(this->GlobalParserState == PARSE_BODY || this->GlobalParserState == PARSE_COMPLETED)
        {
            this->ParseBody();
        }
    }

    bool HttpParsersBase::HaveHeader(QByteArray headerName)
    {
        return this->HeaderIdx.find(headerName.toLower()) != this->HeaderIdx.end();
    }

    QByteArray HttpParsersBase::GetHeaderByName(QByteArray headerName)
    {
        if( this->HeaderIdx.find(headerName.toLower()) != this->HeaderIdx.end())
        {
            return this->HeadersVals.at(this->HeaderIdx[headerName.toLower()]);
        }
        return QByteArray();
    }

    QByteArray HttpParsersBase::GetRaw()
    {
        return this->RawData;
    }

    QByteArray HttpParsersBase::GetRawBody()
    {
        return this->Body;
    }

    QByteArray HttpParsersBase::GetRawHeaders()
    {
        QByteArray output = this->GetFirstLine() + "\r\n";
        int idx = 0;
        for(const QByteArray &headerName: this->HeadersNames)
        {
            output += headerName + ":" + this->HeadersVals.at(idx++) + "\r\n";
        }
        return output;
    }

    QString HttpParsersBase::GetParseFailReason()
    {
        return this->ParseFailReason;
    }

    bool HttpParsersBase::IsValidHttpData()
    {
        return this->GlobalParserState != PARSE_COMPLETED;
    }

    bool HttpParsersBase::IsCompleteHttpData()
    {
        return this->GlobalParserState == PARSE_COMPLETED;
    }

    void HttpParsersBase::clear()
    {
        this->UnprocessedBodyDataReceived.clear();
        this->RawData.clear();
        this->GlobalParserState = PARSE_FIRST_LINE;
        this->ParseFailReason.clear();

        this->HttpVersion.clear();
        this->HeadersNames.clear();
        this->HeadersVals.clear();
        this->HeaderIdx.clear();
        this->Body.clear();
    }

    void HttpParsersBase::ParseHeaders()
    {
        if(!this->UnprocessedBodyDataReceived.contains("\r\n"))
            return;

        QByteArray headersToBeProcessed;

        // Full body received?
        if(this->UnprocessedBodyDataReceived.contains("\r\n\r\n"))
        {
            headersToBeProcessed = this->UnprocessedBodyDataReceived.left(this->UnprocessedBodyDataReceived.indexOf("\r\n\r\n"));
            this->UnprocessedBodyDataReceived.remove(0, this->UnprocessedBodyDataReceived.indexOf("\r\n\r\n") + 4);
            this->GlobalParserState = PARSE_BODY;
        }
        else
        {
            // Complete headers received?
            if(this->UnprocessedBodyDataReceived.endsWith("\r\n"))
            {
                headersToBeProcessed = this->UnprocessedBodyDataReceived;
                this->UnprocessedBodyDataReceived.clear();
            }
            else if(this->UnprocessedBodyDataReceived.contains("\r\n")) // Last header from list is not completed? leave it for next time and process what we have so far
            {
                headersToBeProcessed = this->UnprocessedBodyDataReceived.left(this->UnprocessedBodyDataReceived.lastIndexOf("\r\n"));
                this->UnprocessedBodyDataReceived.remove(0, this->UnprocessedBodyDataReceived.lastIndexOf("\r\n"));
            }
        }

        // Read headers
        for(QByteArray header: SplitByteArray(headersToBeProcessed, "\r\n"))
        {
            if(!header.contains(":"))
            {
                this->GlobalParserState = PARSE_FAILED;
                this->ParseFailReason = "invalid header detected: '" + header + "'";
                return;
            }

            QByteArray headerName = header.left(header.indexOf(':'));
            QByteArray headerValue = header.right(header.length() - 1 - header.indexOf(':'));

            this->HeadersVals.append(headerValue.trimmed());
            this->HeadersNames.append(headerName.trimmed());
            this->HeaderIdx[headerName.trimmed().toLower()] = this->HeadersNames.count() - 1;
        }
    }

    void HttpParsersBase::ParseBody()
    {
        if( this->GetHeaderByName("Transfer-Encoding").toLower().contains("chunked"))
        {
            this->ParseChunkedTransferEncoding();
        }
        else if(this->HaveHeader("Content-Length"))
        {
            this->ParseContentLengthBody();
        }
        else // no body length specified - treat whatever is received as body and wait until connection is closed
        {
            this->Body.append(this->UnprocessedBodyDataReceived);
            this->UnprocessedBodyDataReceived.clear();
            this->UnspecifiedBodyLength = true;
        }
    }

    void HttpParsersBase::ParseChunkedTransferEncoding()
    {
        // Here for the first time. Read first byte length form the body;
        if( ChunkedParsingState == ChunksParserState::CHNK_INVALID )
        {
            ChunkedParsingState = ChunksParserState::CHNK_WAIT_LENGTH;
            this->currentChunkLength = -1;
        }

        // Get the length of the chunk
        if( ChunkedParsingState == ChunksParserState::CHNK_WAIT_LENGTH )
        {
            // Length needs to be terminated with \r\n. If this token is not found, just wait for some more incoming data
            if( this->UnprocessedBodyDataReceived.contains("\r\n") )
            {
                bool ok  = false;
                //this->currentChunkLength = QString(this->UnprocessedBodyDataReceived).split(QRegularExpression("\r\n"))[0].toInt(&ok, 16);
                this->currentChunkLength = QString(this->UnprocessedBodyDataReceived.left(this->UnprocessedBodyDataReceived.indexOf("\r\n"))).toInt(&ok, 16);

                // consume bytes from unprocessed data
                this->UnprocessedBodyDataReceived.remove(0, this->UnprocessedBodyDataReceived.indexOf("\r\n") + 2);

                // If length is invalid, just abort
                if( !ok )
                {
                    this->ParseFailReason = "invalid chunk length received (expected number of bytes in HEX format)";
                    this->GlobalParserState = GlobalParserState::PARSE_FAILED;
                    this->ChunkedParsingState = ChunksParserState::CHNK_INVALID;
                }

                // Chunk length 0 means this was the last packet.
                else if(this->currentChunkLength == 0)
                {
                    this->ChunkedParsingState = ChunksParserState::CHNK_WAIT_FINAL_EOF;
                }

                // Correct length received, switch to next state - waiting for actual content to be transferred
                else
                {
                    this->ChunkedParsingState = ChunksParserState::CHNK_WAIT_CONTENT;
                }
            }
        }

        // Get the content of the chunk
        if( this->ChunkedParsingState == ChunksParserState::CHNK_WAIT_CONTENT )
        {
            // All the data was received
            if( this->UnprocessedBodyDataReceived.length() >= this->currentChunkLength )
            {
                this->Body.append(this->UnprocessedBodyDataReceived.left(this->currentChunkLength));
                //  Consume data from reception buffer
                this->UnprocessedBodyDataReceived.remove(0, this->currentChunkLength);
                // Switch to next state
                this->ChunkedParsingState = ChunksParserState::CHNK_WAIT_CONTENT_EOF;
            }
        }

        // Get chunk EOF
        if( this->ChunkedParsingState == ChunksParserState::CHNK_WAIT_CONTENT_EOF )
        {
            if( this->UnprocessedBodyDataReceived.size() >= 2 )
            {
                if( this->UnprocessedBodyDataReceived[0] == '\r' && this->UnprocessedBodyDataReceived[1] == '\n' )
                {
                    // Consume the EOF and switch to next state
                    this->UnprocessedBodyDataReceived.remove(0, 2);
                    this->ChunkedParsingState = ChunksParserState::CHNK_WAIT_LENGTH;
                }
                else
                {
                    this->ParseFailReason = "invalid chunk EOF received (expected '\\r\\n' after each chunk)";
                    this->GlobalParserState = GlobalParserState::PARSE_FAILED;
                    this->ChunkedParsingState = ChunksParserState::CHNK_INVALID;
                }
            }
        }

        // Get final EOF
        if( this->ChunkedParsingState == ChunksParserState::CHNK_WAIT_FINAL_EOF )
        {
            if( this->UnprocessedBodyDataReceived.size() >= 2 )
            {
                if( this->UnprocessedBodyDataReceived[0] == '\r' && this->UnprocessedBodyDataReceived[1] == '\n' )
                {
                    this->ParseFailReason = "";
                    this->GlobalParserState = GlobalParserState::PARSE_COMPLETED;
                    this->ChunkedParsingState = ChunksParserState::CHNK_PARSE_COMPLETED;
                    this->UnprocessedBodyDataReceived.remove(0, 2);
                }
                else
                {
                    this->ParseFailReason = "final chunk end token is wrong, expected '\\r\\n'";
                    this->GlobalParserState = GlobalParserState::PARSE_FAILED;
                    this->ChunkedParsingState = ChunksParserState::CHNK_INVALID;
                }
            }
        }

        // This function is executed only on events. We need to process all possible events in the loop
        if( (this->UnprocessedBodyDataReceived.contains("\r\n") && (ChunkedParsingState == ChunksParserState::CHNK_WAIT_LENGTH))
        && (this->ChunkedParsingState != ChunksParserState::CHNK_INVALID) && (this->ChunkedParsingState != ChunksParserState::CHNK_PARSE_COMPLETED))
        {
            this->ParseChunkedTransferEncoding();
        }
    }

    void HttpParsersBase::ParseContentLengthBody()
    {
        this->Body.append(this->UnprocessedBodyDataReceived);
        this->UnprocessedBodyDataReceived.clear();

        int expectedLength = QString(this->GetHeaderByName("Content-Length")).toInt();

        if(expectedLength > 0)
        {
            if(this->Body.length() < expectedLength)
            {
                this->ParseFailReason = "incomplete body, expected " + QString::number(expectedLength) + ", received " + QString::number(this->Body.length());
            }
            else
            {
                this->GlobalParserState = PARSE_COMPLETED;
                // Also clear last error when successfully finished
                this->ParseFailReason = "";
            }
        }
        else
        {
            this->GlobalParserState = PARSE_COMPLETED;
            // Also clear last error when successfully finished
            this->ParseFailReason = "";
        }
    }

}}; // Parsers
