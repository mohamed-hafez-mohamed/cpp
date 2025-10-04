/**
 * @file PCControl.hpp
 * @brief Header file for PC Control application
 * 
 * Provides handlers for client requests
 * 
 * @author Mohamed Hafez
 * @version 1.0
 */
#pragma once

#include <string>            ///< For std::string class operations
#include <functional>        ///< For std::function
#include <unordered_map>     ///< For std::unordered_map
#include "Logger.hpp"

/**
 * @namespace App
 * @brief A collection of various application utilities.
 */
namespace App
{
    /**
     * @class PCControl
     * @brief Performs utilities to control PC based on client requests
     */
    class PCControl
    {
        public:
        PCControl();                                      ///< Default constructor
        PCControl(const PCControl&) = delete;             ///< Delete copy constructor
        PCControl& operator=(const PCControl&) = delete;  ///< Delete copy assignment operator
        PCControl( PCControl&&) = delete;                 ///< Delete move constructor
        PCControl& operator=( PCControl&&) = delete;      ///< Delete move assignment operator
        ~PCControl();
        /**
         * @brief Insert a request and its corresponding handler into a lookup table
         * @param request The client request to be handled
         * @param requestHandle The function to handle the specified request
         */
        void insertRequestHandle(std::string request, std::function<void(void)> requestHandle);
        /**
         * @brief Handle the specified client request by invoking the corresponding handler
         * @param request The client request to be handled
         */
        void handleRequest(std::string request);
        private:
        pid_t m_broswerProcessID{-1};     ///< Store broswer process ID
        /**
         * @brief Open default browser
         */
        void openBrowser(); 
        /**
         * @brief Close default browser
         */  
        void closeBrowser();  
        // Create Lookup table for request handlers
        std::unordered_map<std::string, std::function<void(void)>> m_requestHandleTable;
        // Create Logger instance for PC Control logging
        Logger m_PCControlLogger{Logger::Levels::ERROR, "PCCControlLog.log", true}; 
    };

} // namespace App







