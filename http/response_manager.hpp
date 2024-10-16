#ifndef HTTP_RESPONSE_MANAGER
#define HTTP_RESPONSE_MANAGER

#include <filesystem>
#include <fstream>

#include "request.hpp"
#include "response.hpp"
#include "parser.hpp"

namespace http
{
    class ResponseManager
    {
        private:
            ResponseManager()
            {
            }

            ResponseManager(const ResponseManager&) = delete;
            ResponseManager& operator=(const ResponseManager&) = delete;
        
        public:
            static ResponseManager& getResponseManager()
            {
                static ResponseManager responseManager;
                return responseManager;
            }

            ~ResponseManager()
            {
            }

            std::string process(Request request)
            {
                if (request.getMethod() == http::METHOD::GET) {
                    if (request.getUrl() == "/") {
                        return defaultResponse();
                    }
                    return buildResponseFromFile(request.getUrl());
                } else if (request.getMethod() == http::METHOD::POST) {
                    // manage post request
                }

                // manage other unprocessable methods
                return "";
            }

            std::string defaultResponse()
            {
                return buildResponseFromFile("html/welcome.html");
            }

            std::string buildResponseFromFile(const std::string& filePath)
            {
                std::ifstream file("./" + filePath);
                if (!file.is_open()) {
                    return notFoundResponse();
                }

                std::pair<const std::string, const std::string> fileContentAndMimeType = getFileContentAndMimeType(file, filePath);

                Response response(Response::OK, "OK", fileContentAndMimeType.second, fileContentAndMimeType.first);

                return response.get();
            }

            std::string notFoundResponse()
            {
                return buildResponseFromFile("html/404.html");
            }

            std::pair<const std::string, const std::string> getFileContentAndMimeType(std::ifstream& file, const std::string& filePath)
            {
                std::ostringstream ss;
                ss << file.rdbuf();

                std::filesystem::path path = filePath;
                const std::string ext = path.extension();

                return {ss.str(), mimeTypes[ext]};
            }
    };
}

#endif