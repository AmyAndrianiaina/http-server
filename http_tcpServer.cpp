#include "http_tcpServer.hpp"

#include <iostream>
#include <sstream>
#include <fstream>
#include <unistd.h>

#include "http_parser.hpp"

namespace
{
    const int BUFFER_SIZE = 30720;

    void log(const std::string& message)
    {
        std::cout << message << std::endl;
    }

    void exitWithError(const std::string& errorMessage)
    {
        log("ERROR: " + errorMessage);
        exit(1);
    }
}

namespace http
{
    TcpServer::TcpServer(std::string ip_address, int port)
        : m_ip_address(ip_address), m_port(port), m_socket(),
          m_new_socket(), m_incoming_message(), m_socket_address(),
          m_socket_address_len(sizeof(m_socket_address))
    {
        m_socket_address.sin_family = AF_INET;
        m_socket_address.sin_port = htons(m_port);
        m_socket_address.sin_addr.s_addr = inet_addr(m_ip_address.c_str());
        
        if(startServer() != 0)
        {
            std::ostringstream ss;
            ss << "Failed to start server with PORT: " << ntohs(m_socket_address.sin_port);
            log(ss.str());
        }
    }

    TcpServer::~TcpServer()
    {
        closeServer();
    }

    int TcpServer::startServer()
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

    void TcpServer::closeServer()
    {
        close(m_socket);
        close(m_new_socket);
        exit(0);
    }

    void TcpServer::startListen()
    {
        if (listen(m_socket, 20) < 0) 
        {
            exitWithError("Socket listen failed");
        }

        std::ostringstream ss;
        ss << "\n*** Listening on ADDRESS: "
            << inet_ntoa(m_socket_address.sin_addr)
            << " PORT: " << ntohs(m_socket_address.sin_port)
            << " ***\n\n";
        log(ss.str());

        int bytesReceived;

        while (true)
        {
            log("====== Waiting for a new connection ======\n\n\n");
            acceptConnection(m_new_socket);

            char buffer[BUFFER_SIZE] {0};
            bytesReceived = read(m_new_socket, buffer, BUFFER_SIZE);

            if(bytesReceived < 0)
            {
                exitWithError("Failed to read bytes from client socket connection");
            }
            
            Parser& parser = Parser::getParser();
            Info info = parser.parse(buffer, bytesReceived);
            info.print();

            std::ostringstream ss;
            ss << "------ Received Request from client ------\n\n";
            ss << buffer;
            log(ss.str());

            processRequest(info);
            
            sendResponse();

            close(m_new_socket);
        }
    }

    void TcpServer::acceptConnection(int new_socket)
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

    std::string TcpServer::buildResponse()
    {
        return buildResponseFromFile("html/index.html");
    }

    std::string TcpServer::buildResponseFromString()
    {
        std::string htmlFile = "<!DOCTYPE html><html lang=\"en\"><body><h1> HOME </h1><p> Hello from your Server :) </p></body></html>";
        std::ostringstream ss;
        ss << "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " << htmlFile.size() << "\n\n"
           << htmlFile;

        return ss.str();
    }

    std::string TcpServer::buildResponseFromFile(const std::string& filePath)
    {
        std::ifstream file("./" + filePath);
        if (!file.is_open()) {
            return "HTTP/1.1 404 Not Found\nContent-Type: text/plain\nContent-Length: 13\n\n404 Not Found";
        }

        std::ostringstream ss;
        ss << file.rdbuf(); // read file content into the stringstream
        std::string htmlFile = ss.str();

        std::ostringstream response;
        response << "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " << htmlFile.size() << "\n\n"
            << htmlFile;

        return response.str();
    }

    void TcpServer::sendResponse()
    {
        long bytesSent;

        bytesSent = write(m_new_socket, m_server_message.c_str(), m_server_message.size());

        if (bytesSent == m_server_message.size())
        {
            log("------ Server Response sent to client ------\n\n");
        }
        else
        {
            log("Error sending response to client");
        }
    }

    void TcpServer::processRequest(const Info &info)
    {

        if (info.getMethod() == http::METHOD::GET) {
            if (info.getUrl() == "/") {
                m_server_message = buildResponse();
                return;
            }
            m_server_message = buildResponseFromFile(info.getUrl());
        } else if (info.getMethod() == http::METHOD::POST) {
            // manage post request
        }

        // manage unprocessable methods
    }
}