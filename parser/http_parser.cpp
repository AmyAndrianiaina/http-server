#include "http_parser.hpp"

#include <iostream>
#include <sstream> 
#include <string>
#include <vector>


http::Parser &http::Parser::getParser()
{
    static Parser parser;
    return parser;
}

http::Info http::Parser::parse(const char *data, int size)
{
    Info info;

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
        http::METHOD methodEnum = http::stringToMethod(method);
        std::string url = requestLineInfo[1];
        std::string protocolVersion = requestLineInfo[2];
        info.setMethod(methodEnum);
        info.setUrl(url);
        info.setProtocolVersion(protocolVersion);
    }

    return info;
}

const std::string http::Parser::notFoundResponse()
{
    return "HTTP/1.1 404\nContent-Type: text/plain\nContent-Length: 13\n\n404 Not Found";
}

const std::string http::Parser::successResponse(const std::string &content, int size)
{
    std::ostringstream response;
    response << "HTTP/1.1 201\nContent-Type: text/html\nContent-Length: " << size << "\n\n"
            << content;
    return response.str();
}

http::Parser::Parser()
{
    std::cout << "Construct Parser" << std::endl;
}