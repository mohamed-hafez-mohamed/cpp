
/**
 * @file Server.hpp
 * @brief Header file for server application
 * 
 * Provides socket creation, listening to client, accept connection, receive and send data
 * 
 * @author Mohamed Hafez
 * @version 1.0
 */

#pragma once

#include <deque>             ///< For std::deque container
#include <string>            ///< For std::string class operations
#include <cstring>           ///< C string manipulation functions (memset, strlen)
#include <sys/socket.h>      ///< Core socket programming functions (socket, bind, listen, accept)
#include <netinet/in.h>      ///< Internet address family structures (sockaddr_in, INADDR_ANY)
#include <unistd.h>          ///< POSIX operating system API (close function, read/write)
#include "Logger.hpp"        ///< Custom logger class for logging messages

// Configurable parameters
constexpr int SERVER_SOCKET_DOMAIN{AF_INET};    ///< Socket domain: IPv4
constexpr int SERVER_SOCKET_TYPE{SOCK_STREAM};  /// Socket type: TCP
constexpr int SERVER_SOCKET_PROTOCOL{0};        /// Socket protocol: TCP
constexpr int BACKLOG{5};                       ///< Maximum number of pending connections
constexpr int SERVER_BUFFER_SIZE{1024};         ///< Size of the buffer for receiving data
constexpr int SERVER_RECEIVE_FLAG{0};           ///< Standard blocking receive


/**
 * @namespace App
 * @brief A collection of various application utilities.
 */
namespace App
{

/**
 * @class Server
 * @brief Performs server utilities
 */
class Server
{
    public:
        /** 
         * @brief Constructor to initialize and set up the server
         * @param port The port number on which the server will listen for incoming connections
         */
        Server(int port);
        Server(const Server&) = delete;             ///< Delete copy constructor
        Server& operator=(const Server&) = delete;  ///< Delete copy assignment operator
        Server( Server&&) = delete;                 ///< Delete move constructor
        Server& operator=( Server&&) = delete;      ///< Delete move assignment operator
        /**
         * @brief Destroy the Server object 
         */
        ~Server();
        /**
         * @brief Accepts a client connection
         */
        void acceptClientConnection();
        /**
         * @brief Save client requests to message queue
         */
        void saveClientRequests();
        /**
         * @brief Returns the message queue
         * @return The message queue
         */
        const std::deque<std::string>& getMessageQueue() const;
        /**
         * @brief Gets and removes the next request from the queue
         * @return The next request, or empty string if queue is empty
         */
        std::string getNextRequest();   
    private:
        int m_serverfileDescriptor{};                 ///< Server file descriptor
        int m_clientfileDescriptor{};                 ///< Client file descriptor
        struct sockaddr_in m_serverStuctAddress{};    ///< Server address structure
        struct sockaddr_in m_clientStuctAddress{};    ///< Client address structure
        socklen_t m_clientStuctAddressLength{};       ///< Length of client address structure
        std::deque<std::string> m_messageQueue{};     ///< Queue to store messages
        // Create Logger instance for server logging
        Logger m_serverLogger{Logger::Levels::ERROR, "ServerLog.log", true}; 
};
} // namespace App