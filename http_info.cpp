#include "http_info.hpp"
#include <iostream>

namespace http
{
    METHOD stringToMethod(const std::string& meth)
    {
        if (meth == "GET") {
            return METHOD::GET;
        } else if (meth == "POST") {
            return METHOD::POST;
        }

        // Manage invalid method; return GET by default
        return METHOD::GET;
    }

    std::string methodToString(const METHOD meth)
    {
        if (meth == METHOD::GET) {
            return "GET";
        } else if (meth == METHOD::POST) {
            return "POST";
        }

        // Manage invalid method; return POST by default
        return "POST";
    }
}

http::Info::Info(const http::METHOD& method, const std::string& url, const std::string& protocolVersion, const std::unordered_map<std::string, std::string>& headers, const std::string& body)
    : m_method(method), m_url(url), m_protocolVersion(protocolVersion), m_headers(headers), m_body(body)
{
}

http::Info::Info()
{
}

http::Info::~Info()
{
}

const http::METHOD http::Info::getMethod() const
{
    return m_method;
}

void http::Info::setMethod(const http::METHOD &method)
{
    m_method = method;
}

const std::string http::Info::getUrl() const
{
    return m_url;
}

void http::Info::setUrl(const std::string &url)
{
    m_url = url;
}

const std::string http::Info::getProtocolVersion() const
{
    return m_protocolVersion;
}

void http::Info::setProtocolVersion(const std::string &protocolVersion)
{
    m_protocolVersion = protocolVersion;
}

const std::unordered_map<std::string, std::string> http::Info::getHeaders() const
{
    return m_headers;
}

const std::string http::Info::getBody() const
{
    return m_body;
}

void http::Info::print() const
{
    std::cout << "Method: " << http::methodToString(getMethod()) << std::endl;
    std::cout << "Url: " << getUrl() << std::endl;
    std::cout << "Protocol Version: " << getProtocolVersion() << std::endl;
}
