/**
 * @file Server.cpp
 * @brief Source file for server application that implements socket utilities
 *
 * Provides socket creation, listening to client, accept connection, receive and send data
 *
 * @author Mohamed Hafez
 * @version 1.0
 */

#include <iostream>       ///< For input/output operations (std::cout, std::cerr)
#include <algorithm>      ///< For std::transform
#include <cctype>         ///< For std::tolower
#include "Logger.hpp"
#include "Server.hpp"

/**
 * @namespace App
 * @brief A collection of various application utilities.
 */
namespace App
{
/**
 * @brief Constructor to initialize and set up the server
 * @param port The port number on which the server will listen for incoming connections
 */
Server::Server(int port)
{
    std::cout << "=== STEP 1: CREATING SOCKET ===\n";
    // Create socket with IP:IPv4 and Protocol:TCP
    m_serverfileDescriptor = socket(SERVER_SOCKET_DOMAIN, SERVER_SOCKET_TYPE, SERVER_SOCKET_PROTOCOL);
    // Check if socket creation is successful
    if(-1 == m_serverfileDescriptor)
    {
        // Log error
        m_serverLogger.error("An error occurred while creating socket");
        exit(EXIT_FAILURE);
    }
    std::cout << "Socket created successfully with file descriptor: " << m_serverfileDescriptor << '\n';
    std::cout << "=== STEP 2: BINDING SOCKET TO IP/PORT ===\n";
    // Bind socket to IP/Port
    m_serverStuctAddress.sin_family = SERVER_SOCKET_DOMAIN;          ///< IPv4
    m_serverStuctAddress.sin_addr.s_addr = INADDR_ANY;        ///< Accept connections from any IP address (IPv4 or IPv6)
    m_serverStuctAddress.sin_port = htons(port);   ///< Port
    // Bind the socket to the specified IP/Port
    int SockBindState = bind(m_serverfileDescriptor, (struct sockaddr*)&m_serverStuctAddress, sizeof(m_serverStuctAddress));
    if(-1 == SockBindState)
    {
        // Log error
        m_serverLogger.error("An error occurred while binding socket to IP/Port");
        // Close the socket before exiting
        close(m_serverfileDescriptor);
        exit(EXIT_FAILURE);
    }
    std::cout << "Socket bound to IP/Port successfully\n";
    std::cout << "=== STEP 3: LISTENING TO CLIENT ===\n";
    // Listen to client
    int ListenToClientState = listen(m_serverfileDescriptor, BACKLOG);
    if(-1 == ListenToClientState)
    {
        // Log error
        m_serverLogger.error("An error occurred while listening to client");
        // Close the socket before exiting
        close(m_serverfileDescriptor);
        exit(EXIT_FAILURE);
    }
    std::cout << "Server is listening to client on port: " << port << '\n';
    std::cout << "Maximum pending connections: " << BACKLOG << "\n";
}
/**
 * @brief Accepts a client connection
 */
void Server::acceptClientConnection()
{
    std::cout << "\n=== STEP 5: ACCEPTING CLIENT CONNECTION ===\n";
    m_clientStuctAddressLength = sizeof(m_clientStuctAddress);
    // Accept client connection
    m_clientfileDescriptor = accept(m_serverfileDescriptor, (struct sockaddr*)&m_clientStuctAddress, &m_clientStuctAddressLength);
    // check if accepting client connection is successful
    if(-1 == m_clientfileDescriptor)
    {
        // Log error
        m_serverLogger.error("An error occurred while accepting client connection");
        // Close the server socket before exiting
        close(m_serverfileDescriptor);
        exit(EXIT_FAILURE);
    }
    std::cout << "Client connected successfully with file descriptor: " << m_clientfileDescriptor << '\n';
}
/**
 * @brief Save client requests to message queue
 */
void Server::saveClientRequests()
{
    std::cout << "\n=== STEP 6: HANDLING CLIENT COMMUNICATION ===\n";
    char Buffer[SERVER_BUFFER_SIZE]{};
    // Clear the buffer before receiving new data
    memset(Buffer, 0, sizeof(Buffer));
    ssize_t NumberOfReceivedBytes = recv(m_clientfileDescriptor, Buffer, sizeof(Buffer) - 1, SERVER_RECEIVE_FLAG);
    // Check if receiving data is successful
    if(-1 == NumberOfReceivedBytes)
    {
        // // Log error
        // m_serverLogger.error("An error occurred while receiving data from client");
        // // Close the sockets before exiting
        // close(m_clientfileDescriptor);
        // close(m_serverfileDescriptor);
    }
    else if(0 == NumberOfReceivedBytes)
    {
        std::cout << "Client disconnected gracefully.\n";
        // Close the client socket
        close(m_clientfileDescriptor);
        //break;
    }
    else
    {
        // Null-terminate the received data to make it a valid C-string
        Buffer[NumberOfReceivedBytes] = '\0';
        std::string ReceivedMessage(Buffer);
        // Create a lowercase version of the message for case-insensitive comparison
        std::string ReceivedMessageInLowerCase = ReceivedMessage;  // Initialize with same size
        // Convert the message to lowercase for case-insensitive comparison
        std::transform(ReceivedMessage.begin(), ReceivedMessage.end(),
                         ReceivedMessageInLowerCase.begin(),
                         [](unsigned char Letter) { return std::tolower(Letter); });
        // Save the lowercase version to the message queue for consistent comparison
        m_messageQueue.push_back(ReceivedMessageInLowerCase);
        std::cout << "Received message: " << ReceivedMessage << " (stored as: " << ReceivedMessageInLowerCase << ")\n";
        std::string Acknowledgment = "Message received\n";
        // Send acknowledgment back to the client
        ssize_t NumberOfSentBytes = send(m_clientfileDescriptor, Acknowledgment.c_str(), Acknowledgment.length(), 0);
        if(-1 == NumberOfSentBytes)
        {
            // Log error
            m_serverLogger.error("An error occurred while sending acknowledgment to client");
            // Close the sockets before exiting
            close(m_clientfileDescriptor);
            close(m_serverfileDescriptor);
                //break;
        }
        std::cout << "Sent acknowledgment to client: " << Acknowledgment << '\n';
        if(("exit" == ReceivedMessage) || ("quit" == ReceivedMessage))
        {
            std::cout << "Exit command received. Closing connection.\n";
            // Close the client socket
            close(m_clientfileDescriptor);
            //break;
        }
    }
}

std::string Server::getNextRequest()
{
    if (m_messageQueue.empty())
    {
        return "";
    }
    std::string request = m_messageQueue.front();
    m_messageQueue.pop_front();
    return request;
}

/**
 * @brief Returns the message queue
 * @return The message queue
 */
const std::deque<std::string>& Server::getMessageQueue() const
{
    return m_messageQueue;
}

Server::~Server()
{
    std::cout << "\n=== STEP 7: CLOSING SOCKETS ===\n";
    // Close the client socket if it's open
    if(m_clientfileDescriptor != -1)
    {
        close(m_clientfileDescriptor);
        std::cout << "Client socket closed successfully\n";
    }
    // Close the server socket
    if(m_serverfileDescriptor != -1)
    {
        close(m_serverfileDescriptor);
        std::cout << "Server socket closed successfully\n";
    }
    std::cout << "Server shutdown complete.\n";
}

} // namespace App
