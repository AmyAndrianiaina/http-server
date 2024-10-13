#ifndef INCLUDED_HTTP_TCPSERVER_LINUX
#define INCLUDED_HTTP_TCPSERVER_LINUX

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string>

#include "../parser/http_parser.hpp"

namespace http
{
    class TcpServer
    {
        public:
            TcpServer(std::string ip_address, int port);
            ~TcpServer();
            void startListen();

        private:
            std::string m_ip_address;
            int m_port;
            int m_socket;
            int m_new_socket;
            long m_incoming_message;
            struct sockaddr_in m_socket_address;
            unsigned int m_socket_address_len;
            std::string m_server_message;

            int startServer();
            void closeServer();
            void listenToConnection();
            std::pair<const char*, int> readConnectionPacket();
            Info parseHttpRequest(std::pair<const char*, int>);
            void acceptConnection(int new_socket);
            std::string welcomeResponse();
            std::pair<const std::string, const std::string> getFileContentAndMimeType(std::ifstream& file, const std::string& filePath);
            std::string notFoundResponse();
            std::string buildResponseFromString();
            std::string buildResponseFromFile(const std::string& filePath);
            void sendResponse();
            void processRequest(const Info& info);

            const std::string getServerInfo() const;
            const std::string getRequestInfo(const char* buffer) const;
            const std::string processPhp(const std::string filePath) const;
    };
}

#endif