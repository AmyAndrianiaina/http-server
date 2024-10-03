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


http::Parser::Parser()
{
    std::cout << "Construct Parser" << std::endl;
}

