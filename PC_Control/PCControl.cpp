/**
 * @file PCControl.cpp
 * @brief Header file for PC Control application
 * 
 * Provides handlers for client requests
 * 
 * @author Mohamed Hafez
 * @version 1.0
 */

#include <iostream>          ///< For std::cout and std::cerr
#include <string>            ///< For std::string class operations
#include <fstream>           ///< For file operations
#include <functional>        ///< For std::function
#include <cstdlib>           ///< For general utilities
#include <signal.h>          ///< For kill signals
#include <sys/stat.h>        ///< For chmod
#include <unistd.h>          ///< For fork() and execlp() - UNIX system calls
#include <sys/wait.h>        ///< For waitpid() - process waiting functions
#include "PCControl.hpp"     ///< For std::function
#include "Logger.hpp"

const char* PID_FILE = "ProcessID.pid";

/**
 * @namespace App
 * @brief A collection of various application utilities.
 */
namespace App
{

PCControl::PCControl()
{
    m_requestHandleTable["open_browser"] = std::bind(&PCControl::openBrowser, this);
    m_requestHandleTable["close_browser"] = std::bind(&PCControl::closeBrowser, this);
}
/**
 * @brief Insert a request and its corresponding handler into a lookup table
 * @param request The client request to be handled
 * @param requestHandle The function to handle the specified request
 */
void PCControl::insertRequestHandle(std::string request, std::function<void(void)> requestHandle)
{
    m_requestHandleTable[request] = requestHandle;
}
/**
 * @brief Handle the specified client request by invoking the corresponding handler
 * @param request The client request to be handled
 */
void PCControl::handleRequest(std::string request)
{
    // Remove any whitespace or newlines from the request
    request.erase(0, request.find_first_not_of(" \n\r\t"));  // left trim
    request.erase(request.find_last_not_of(" \n\r\t") + 1);  // right trim
    //Check if the request exists in the lookup table
    if(m_requestHandleTable.find(request) == m_requestHandleTable.end())
    {
        // Log error
        std::cout << "No handler found for request: \"" << request << "\"" << std::endl;
        m_PCControlLogger.error("No handler found for request: " + request);
    }
    else
    {
        std::cout << "Handler found! Executing request: \"" << request << "\"" << std::endl;
        // Invoke the handler function associated with the request
        m_requestHandleTable[request]();
    }
}

/**
 * @brief Open default browser
 */
 void PCControl::openBrowser()
 {
    std::cout << "Launching Firefox..." << std::endl;
    // Creates a new process by duplicating the current process
    pid_t ProcessID = fork();
    // Check if fork() was successful
    if (ProcessID == -1)
    {
        // Fork failed
        std::cout << "Error: Failed to fork process" << std::endl;
        // Log error
        m_PCControlLogger.error("Error: Failed to fork process");
    }
    else if (ProcessID == 0)
    {
        // Child process
        // Try to execute Firefox
        execlp("firefox", "firefox", nullptr);
        // If execlp returns, it failed
        std::cout << "Error: Failed to launch Firefox" << std::endl;
        std::cout << "Make sure Firefox is installed and in your PATH" << std::endl;
        m_PCControlLogger.error("Error: Failed to launch Firefox");
        m_PCControlLogger.error("Make sure Firefox is installed and in your PATH");
    }
    else
    {
        m_broswerProcessID = ProcessID;
        std::ofstream ProcessIdFileHandler(PID_FILE);
        if(ProcessIdFileHandler.is_open())
        {
            ProcessIdFileHandler << ProcessID;
            ProcessIdFileHandler.close();
            std::cout << "Firefox opened with PID: " << ProcessID << std::endl;
        }
        else
        {
            std::cout << "Error: Unable to open file to write Process ID" << std::endl;
            m_PCControlLogger.error("Error: Unable to open file to write Process ID");
            kill(ProcessID, SIGTERM);
        }
    }
 }

/**
 * @brief Close default browser
 */
 void PCControl::closeBrowser()
 {
    std::cout << "Attempting to close Firefox..." << std::endl;
    // Get the browser process ID
    pid_t ProcessID = m_broswerProcessID;
    if (ProcessID == -1)
    {
        // Try to get PID from file if not stored in memory
        std::ifstream ProcessIdFileHandler(PID_FILE);
        if(ProcessIdFileHandler.is_open())
        {
            ProcessIdFileHandler >> ProcessID;
            ProcessIdFileHandler.close();
        }
        else
        {
            std::string error = "Error: Unable to determine Firefox process ID";
            std::cout << error << std::endl;
            m_PCControlLogger.error(error);
            return;
        }
    }
    kill(ProcessID, SIGTERM);
 }

PCControl::~PCControl()
{
    pid_t ProcessID = m_broswerProcessID;
    if (ProcessID != -1)
    {
        kill(ProcessID, SIGKILL);
    } 
}
} // namespace App

