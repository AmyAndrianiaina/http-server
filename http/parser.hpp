#ifndef HTTP_PARSER
#define HTTP_PARSER

#include <iostream>
#include <sstream> 
#include <string>
#include <vector>

#include "request.hpp"

namespace http
{

    class Parser
    {
        private:
            Parser()
            {
            }

            Parser(const Parser&) = delete;
            Parser& operator=(const Parser&) = delete;
        
        public:
            static Parser& getParser()
            {
                static Parser parser;
                return parser;
            }

            http::Request parseRequest(const char *data, int size)
            {
                http::Request request;

                std::istringstream stream(data);
                std::vector<std::string> arr;

                std::string token;

                while (std::getline(stream, token, '\n')) {
                    arr.push_back(token);
                }

                if (arr.size() > 0) {
                    std::string requestLine = arr[0];
                    std::array<std::string, 3> requestLineInfo;
                    std::string infos;

                    std::istringstream ss(requestLine);

                    int i = 0;
                    while (std::getline(ss, infos, ' ')) {
                        requestLineInfo[i] = infos;
                        i++;
                    }


                    std::string method = requestLineInfo[0];
                    http::METHOD methodEnum = http::STRING_TO_METHOD.at(method);
                    std::string url = requestLineInfo[1];
                    std::string protocolVersion = requestLineInfo[2];
                    request.setMethod(methodEnum);
                    request.setUrl(url);
                    request.setProtocolVersion(protocolVersion);
                }

                return request;
            }

            const std::string notFoundResponse()
            {
                return "HTTP/1.1 404\nContent-Type: text/plain\nContent-Length: 13\n\n404 Not Found";
            }

            const std::string successResponse(const std::string &content, int size)
            {
                std::ostringstream response;
                response << "HTTP/1.1 201\nContent-Type: text/html\nContent-Length: " << size << "\n\n"
                        << content;
                return response.str();
            }
    };
}

#endif