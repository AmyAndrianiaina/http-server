#ifndef HTTP_REQUEST
#define HTTP_REQUEST

#include <iostream>
#include <string>
#include <unordered_map>

namespace http
{
    enum class METHOD
    {
        GET,
        POST
    };

    const static std::unordered_map<http::METHOD, std::string> METHOD_TO_STRING = {
        { METHOD::GET, "GET" }
    };

    const static std::unordered_map<std::string, http::METHOD> STRING_TO_METHOD = {
        { "GET", METHOD::GET }
    };

    class Request
    {
        private:
            http::METHOD m_method;
            std::string m_url;
            std::string m_protocolVersion;
            std::unordered_map<std::string, std::string> m_headers;
            std::string m_body;
        public:
            Request(const std::string method, const std::string& url, const std::string& protocolVersion, const std::unordered_map<std::string, std::string>& headers, const std::string& body)
                : m_url(url), m_protocolVersion(protocolVersion), m_headers(headers), m_body(body)
            {
                if (http::STRING_TO_METHOD.find(method) == http::STRING_TO_METHOD.end()) {
                    // error message
                }
            }

            Request()
            {
            }

            ~Request()
            {
            }

            const http::METHOD getMethod() const
            {
                return m_method;
            }

            void setMethod(const http::METHOD &method)
            {
                m_method = method;
            }

            const std::string getUrl() const
            {
                return m_url;
            }

            void setUrl(const std::string &url)
            {
                m_url = url;
            }

            const std::string getProtocolVersion() const
            {
                return m_protocolVersion;
            }

            void setProtocolVersion(const std::string &protocolVersion)
            {
                m_protocolVersion = protocolVersion;
            }

            const std::unordered_map<std::string, std::string> getHeaders() const
            {
                return m_headers;
            }

            const std::string getBody() const
            {
                return m_body;
            }

            void print() const
            {
                std::cout << "Method: " << http::METHOD_TO_STRING.at(getMethod()) << std::endl;
                std::cout << "Url: " << getUrl() << std::endl;
                std::cout << "Protocol Version: " << getProtocolVersion() << std::endl;
            }
    };
}

#endif