#include "http_tcpServer.hpp"

#include <iostream>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <unistd.h>
#include <unordered_map>

#include "../response/http_response.hpp"
#include "../helper/LogHelper.hpp"

namespace http
{
    /**
     * Max Read Buffer size
     */
    const int BUFFER_SIZE = 30720;

    /**
     * Max connection request size
     */
    const int CONNECTION_REQUEST_SIZE = 20;

    /**
     * TCP Server constructor
     */
    TcpServer::TcpServer(std::string ip_address, int port)
        : m_ip_address(ip_address), m_port(port), m_socket(),
          m_new_socket(), m_incoming_message(), m_socket_address(),
          m_socket_address_len(sizeof(m_socket_address))
    {
        // building socket address
        m_socket_address.sin_family = AF_INET;
        m_socket_address.sin_port = htons(m_port);
        m_socket_address.sin_addr.s_addr = inet_addr(m_ip_address.c_str());
        
        // starting server
        if(startServer() != 0)
        {
            std::ostringstream ss;
            ss << "Failed to start server with PORT: " << ntohs(m_socket_address.sin_port);
            log(ss.str());
        }
    }

    /**
     * TPC Server desctructor
     */
    TcpServer::~TcpServer()
    {
        closeServer();
    }

    /**
     * Start Server
     * 
     * Open socket
     * Bind to socket address
     */
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

    /**
     * Close Server
     * 
     * Close all sockets
     * Exit
     */
    void TcpServer::closeServer()
    {
        close(m_socket);
        close(m_new_socket);
        exit(0);
    }

    /**
     * Get Server Info in string
     */
    const std::string TcpServer::getServerInfo() const
    {
        std::ostringstream ss;
        ss << "\n*** Listening on ADDRESS: "
            << inet_ntoa(m_socket_address.sin_addr)
            << " PORT: " << ntohs(m_socket_address.sin_port)
            << " ***\n\n";

        return ss.str();
    }


    /**
     * Get Request Info in string
     */
    const std::string TcpServer::getRequestInfo(const char* buffer) const
    {
        std::ostringstream ss;
        ss << "------ Received Request from client ------\n\n";
        ss << buffer;
        return ss.str();
    }


    void TcpServer::listenToConnection()
    {
        if (listen(m_socket, CONNECTION_REQUEST_SIZE) < 0)
        {
            exitWithError("Socket listen failed");
        }
    }

    std::pair<const char*, int> TcpServer::readConnectionPacket()
    {
        char buffer[BUFFER_SIZE] {0};
        int bytesReceived = read(m_new_socket, buffer, BUFFER_SIZE);

        if (bytesReceived < 0)
        {
            exitWithError("Failed to read bytes from client socket connection");
        }

        return {buffer, bytesReceived};
    } 

    Info TcpServer::parseHttpRequest(std::pair<const char*, int> bufferInfo)
    {
        Info info = Parser::getParser().parse(bufferInfo.first, bufferInfo.second);
        info.print();

        return info;
    }

    void TcpServer::startListen()
    {
        listenToConnection();

        log(getServerInfo());

        int bytesReceived;

        while (true)
        {
            log("====== Waiting for a new connection ======\n\n\n");
            acceptConnection(m_new_socket);

            std::pair<const char*, int> bufferInfo = readConnectionPacket();

            Info info = parseHttpRequest(bufferInfo); 

            log(getRequestInfo(bufferInfo.first));

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

    /**
     * Default response from file
     * TODO:
     *  - naming function
     */
    std::string TcpServer::welcomeResponse()
    {
        return buildResponseFromFile("html/welcome.html");
    }

    /**
     * Default response from string
     */
    std::string TcpServer::buildResponseFromString()
    {
        std::string htmlFile = "<!DOCTYPE html><html lang=\"en\"><body><h1> HOME </h1><p> Hello from your Server :) </p></body></html>";
        std::ostringstream ss;
        ss << "HTTP/1.1 200 \nContent-Type: text/html\nContent-Length: " << htmlFile.size() << "\n\n"
           << htmlFile;

        return ss.str();
    }

    std::pair<const std::string, const std::string> TcpServer::getFileContentAndMimeType(std::ifstream& file, const std::string& filePath)
    {
        std::ostringstream ss;
        ss << file.rdbuf();
        
        std::filesystem::path path = filePath;
        const std::string ext = path.extension();

        return {ss.str(), mimeTypes[ext]};
    }

    std::string TcpServer::notFoundResponse()
    {
        return buildResponseFromFile("html/404.html");
    }

    /**
     * Build response from file
     */
    std::string TcpServer::buildResponseFromFile(const std::string& filePath)
    {
        std::ifstream file("./" + filePath);
        if (!file.is_open()) {
            return notFoundResponse();
        }
        
        std::pair<const std::string, const std::string> fileContentAndMimeType = getFileContentAndMimeType(file, filePath);

        Response response(Response::OK, "OK", fileContentAndMimeType.second, fileContentAndMimeType.first);

        return response.get();
    }

    /**
     * Send Response
     */
    void TcpServer::sendResponse()
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

    void TcpServer::processRequest(const Info &info)
    {

        if (info.getMethod() == http::METHOD::GET) {
            if (info.getUrl() == "/") {
                m_server_message = welcomeResponse();
                return;
            }
            m_server_message = buildResponseFromFile(info.getUrl());
        } else if (info.getMethod() == http::METHOD::POST) {
            // manage post request
        }

        // manage other unprocessable methods
    }
}