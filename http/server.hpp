#ifndef HTTP_SERVER
#define HTTP_SERVER

#include <iostream>
#include <sstream>
#include <unistd.h>
#include <unordered_map>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string>

#include "../helper/log_helper.hpp"
#include "request_manager.hpp"
#include "response_manager.hpp"

namespace http
{
    class Server
    {
        private:
            std::string m_ip_address;
            int m_port;
            int m_socket;
            int m_new_socket;
            long m_incoming_message;
            struct sockaddr_in m_socket_address;
            unsigned int m_socket_address_len;
            std::string m_server_message;

        public:
            const int MAX_RECEIVED_BUFFER_SIZE = 30720;
            const int MAX_CONNECTION_REQUEST= 20;

            Server(std::string ip_address, int port)
                : m_ip_address(ip_address), m_port(port), m_socket(),
                  m_new_socket(), m_incoming_message(), m_socket_address(),
                  m_socket_address_len(sizeof(m_socket_address))
            {
                m_socket_address.sin_family = AF_INET;
                m_socket_address.sin_port = htons(m_port);
                m_socket_address.sin_addr.s_addr = inet_addr(m_ip_address.c_str());

                if(init() != 0)
                {
                    std::ostringstream ss;
                    ss << "Failed to start server with PORT: " << ntohs(m_socket_address.sin_port);
                    log(ss.str());
                }
            }

            Server()
            {
                stop();
            }

            int init()
            {
                m_socket = socket(AF_INET, SOCK_STREAM, 0);
                if (m_socket < 0)
                {
                    exitWithError("Cannot create socket");
                    return 1;
                }

                // Bind the socket to the socket address
                if (bind(m_socket, (sockaddr*) &m_socket_address, m_socket_address_len) < 0)
                {
                    exitWithError("Cannot bind socket to address");
                    return 1;
                }

                return 0;
            }

            void stop()
            {
                close(m_socket);
                close(m_new_socket);
                exit(0);
            }

            const std::string getServerInfo() const
            {
                std::ostringstream ss;
                ss << "\n*** Listening on ADDRESS: "
                    << inet_ntoa(m_socket_address.sin_addr)
                    << " PORT: " << ntohs(m_socket_address.sin_port)
                    << " ***\n\n";

                return ss.str();
            }

            const std::string getRequestInfo(const char* buffer) const
            {
                std::ostringstream ss;
                ss << "------ Received Request from client ------\n\n";
                ss << buffer;
                return ss.str();
            }

            void listenToConnection()
            {
                if (listen(m_socket, MAX_CONNECTION_REQUEST) < 0)
                {
                    exitWithError("Socket listen failed");
                }
            }

            std::pair<const char*, int> readConnectionPacket()
            {
                char buffer[MAX_RECEIVED_BUFFER_SIZE] {0};
                int bytesReceived = read(m_new_socket, buffer, MAX_RECEIVED_BUFFER_SIZE);

                if (bytesReceived < 0)
                {
                    exitWithError("Failed to read bytes from client socket connection");
                }

                return {buffer, bytesReceived};
            } 

            void start()
            {
                listenToConnection();

                log(getServerInfo());

                int bytesReceived;

                while (true)
                {
                    log("====== Waiting for a new connection ======\n\n\n");
                    acceptConnection(m_new_socket);

                    std::pair<const char*, int> bufferInfo = readConnectionPacket();

                    Request request = RequestManager::getRequestManager().process(bufferInfo); 
                    m_server_message = ResponseManager::getResponseManager().process(request);

                    sendResponse();

                    close(m_new_socket);
                }
            }

            void acceptConnection(int new_socket)
            {
                m_new_socket = accept(m_socket, (sockaddr*) &m_socket_address, (socklen_t*) &m_socket_address_len);
                if (m_new_socket < 0)
                {
                    std::ostringstream ss;
                    ss << 
                    "Server Failed to accept incoming connection from ADDRESS: "
                    << inet_ntoa(m_socket_address.sin_addr) <<  "; PORT: "
                    << ntohs(m_socket_address.sin_port);
                    exitWithError(ss.str());
                }
            }

            void sendResponse()
            {
                long bytesSent;

                bytesSent = write(m_new_socket, m_server_message.c_str(), m_server_message.size());

                if (bytesSent == m_server_message.size())
                {
                    log("------ Server Response sent to client ------\n\n");
                    log(m_server_message + "\n\n");
                }
                else
                {
                    log("Error sending response to client");
                }
            }
    };
}

#endif