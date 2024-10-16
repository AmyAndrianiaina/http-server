#ifndef HTTP_RESPONSE
#define HTTP_RESPONSE

#include <unordered_map>
#include <string>
#include <sstream>

namespace http
{
    std::unordered_map<std::string, std::string> mimeTypes = {
        { ".html", "text/html" },
        { ".htm",  "text/html" },
        { ".css",  "text/css" },
        { ".js",   "application/javascript" },
        { ".json", "application/json" },
        { ".xml",  "application/xml" },
        { ".jpg",  "image/jpeg" },
        { ".jpeg", "image/jpeg" },
        { ".png",  "image/png" },
        { ".gif",  "image/gif" },
        { ".bmp",  "image/bmp" },
        { ".webp", "image/webp" },
        { ".svg",  "image/svg+xml" },
        { ".ico",  "image/x-icon" },
        { ".tiff", "image/tiff" },
        { ".tif",  "image/tiff" },
        { ".mp3",  "audio/mpeg" },
        { ".wav",  "audio/wav" },
        { ".ogg",  "audio/ogg" },
        { ".mp4",  "video/mp4" },
        { ".mkv",  "video/x-matroska" },
        { ".mov",  "video/quicktime" },
        { ".avi",  "video/x-msvideo" },
        { ".zip",  "application/zip" },
        { ".tar",  "application/x-tar" },
        { ".gz",   "application/gzip" },
        { ".pdf",  "application/pdf" },
        { ".txt",  "text/plain" },
        { ".rtf",  "application/rtf" },
        { ".csv",  "text/csv" },
        { ".woff", "font/woff" },
        { ".woff2","font/woff2" },
        { ".eot",  "application/vnd.ms-fontobject" },
        { ".ttf",  "font/ttf" },
        { ".otf",  "font/otf" }
    };
    
    class Response
    {
        public:
            const static int OK = 200;
            const static int NOT_FOUND = 404;

            Response()
                : m_status_code(OK)
            {}

            Response(int status_code)
                : m_status_code(status_code)
            {}

            Response(int status_code, const std::string& status_reason)
                : m_status_code(status_code)
                , m_status_reason(status_reason)
            {}

            Response(int status_code, const std::string& status_reason, const std::string& content_type)
                : m_status_code(status_code)
                , m_status_reason(status_reason)
                , m_content_type(content_type)
            {}

            Response(int status_code, const std::string& status_reason, const std::string& content_type, const std::string& content)
                : m_status_code(status_code)
                , m_status_reason(status_reason)
                , m_content_type(content_type)
                , m_content(content)
            {}

            std::string get()
            {
                std::ostringstream response;
                response << "HTTP/1.1 " << m_status_code;

                if (!m_status_reason.empty())
                    response << ' ' << m_status_reason;
                
                if (!m_content_type.empty())
                    response << '\n' << "Content-Type: " << m_content_type;

                if (!m_content.empty())
                    response << '\n' << "Content-Length: " << m_content.size() << "\n\n"
                        << m_content;

                return response.str();
            }

        private:
            int m_status_code;
            std::string m_status_reason;
            std::string m_content_type;
            std::string m_content;
    };
}

#endif