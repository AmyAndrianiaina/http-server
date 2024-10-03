#ifndef HTTP_PARSER
#define HTTP_PARSER

#include "http_info.hpp"

namespace http
{
    class Parser
    {
        public:
            static Parser& getParser();
            Info parse(const char* data, int size);
        
        private:
            Parser();
            Parser(const Parser&) = delete;
            Parser& operator=(const Parser&) = delete;
            void setRequestLine(Info& info);
    };
}

#endif