#ifndef HTTP_INFO
#define HTTP_INFO

#include <string>
#include <unordered_map>

namespace http
{
    enum class METHOD
    {
        GET,
        POST
    };

    METHOD stringToMethod(const std::string& meth);
    std::string methodToString(const METHOD meth);

    class Info
    {
        public:
            Info();
            Info(const http::METHOD& method, const std::string& url, const std::string& protocolVersion, const std::unordered_map<std::string, std::string>& headers, const std::string& body);
            ~Info();
            const http::METHOD getMethod() const;
            void setMethod(const http::METHOD& method);
            const std::string getUrl() const;
            void setUrl(const std::string& method);
            const std::string getProtocolVersion() const;
            void setProtocolVersion(const std::string& method);
            const std::unordered_map<std::string, std::string> getHeaders() const;
            const std::string getBody() const;
            void print() const;

        private:
            http::METHOD m_method;
            std::string m_url;
            std::string m_protocolVersion;
            std::unordered_map<std::string, std::string> m_headers;
            std::string m_body;
    };
}

#endif
