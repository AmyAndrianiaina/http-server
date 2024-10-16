#ifndef HTTP_REQUEST_MANAGER
#define HTTP_REQUEST_MANAGER

#include "request.hpp"
#include "parser.hpp"

namespace http
{
    class RequestManager
    {
        private:
            RequestManager()
            {
            }

            RequestManager(const RequestManager&) = delete;
            RequestManager& operator=(const RequestManager&) = delete;
        
        public:
            static RequestManager& getRequestManager()
            {
                static RequestManager requestManager;
                return requestManager;
            }

            ~RequestManager()
            {
            }

            http::Request process(std::pair<const char*, int> request)
            {
                return http::Parser::getParser().parseRequest(request.first, request.second);
            }
    };
}

#endif