#ifndef _PARSER_URI_H_
#define _PARSER_URI_H_

#include <QString>
#include <uriparser/Uri.h>

namespace Services { namespace Parsers {
    class Uri
    {
    public:
        Uri(std::string uri): uri_(uri)
        {
            UriParserStateA state_;
            state_.uri = &uriParse_;
            isValid_   = uriParseUriA(&state_, uri_.c_str()) == URI_SUCCESS;
        }

        Uri(QString uri): uri_(uri.toStdString())
        {
            UriParserStateA state_;
            state_.uri = &uriParse_;
            isValid_   = uriParseUriA(&state_, uri_.c_str()) == URI_SUCCESS;
        }


        ~Uri() { uriFreeUriMembersA(&uriParse_); }

        bool isValid() const { return isValid_; }

//        std::string scheme()   const { return fromRange(uriParse_.scheme); }
//        std::string host()     const { return fromRange(uriParse_.hostText); }
//        std::string port()     const { return fromRange(uriParse_.portText); }
//        std::string path()     const { return fromList(uriParse_.pathHead, "/"); }
//        std::string query()    const { return fromRange(uriParse_.query); }
//        std::string fragment() const { return fromRange(uriParse_.fragment); }

        QString scheme()   const { return QString::fromStdString(fromRange(uriParse_.scheme)); }
        QString host()     const { return QString::fromStdString(fromRange(uriParse_.hostText)); }
        QString port()     const { return QString::fromStdString(fromRange(uriParse_.portText)); }
        QString path()     const { return QString::fromStdString(fromList(uriParse_.pathHead, "/")); }
        QString query()    const { return QString::fromStdString(fromRange(uriParse_.query)); }
        QString fragment() const { return QString::fromStdString(fromRange(uriParse_.fragment)); }

    private:
        std::string uri_;
        UriUriA     uriParse_;
        bool        isValid_;

        std::string fromRange(const UriTextRangeA & rng) const
        {
            return std::string(rng.first, rng.afterLast);
        }

        std::string fromList(UriPathSegmentA * xs, const std::string & delim) const
        {
            UriPathSegmentStructA * head(xs);
            std::string accum;

            while (head)
            {
                accum += delim + fromRange(head->text);
                head = head->next;
            }

            return accum;
        }
    };

}} // Namespaces

#endif //_PARSER_URI_H_
