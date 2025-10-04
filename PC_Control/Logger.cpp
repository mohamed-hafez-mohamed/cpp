/**
 ********************************************************************************
 * @file    Logger.cpp
 * @author  MHafez
 * @date    26 August 2025
 * @brief   This file implements the Logger class interfaces
 ********************************************************************************
 */

/************************************
 * INCLUDES
 ************************************/
#include <iostream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <sstream>
#include "Logger.hpp"

/************************************
 * NAMESPACES
 ************************************/

 /**
 * @namespace App
 * @brief A collection of various application utilities.
 */
namespace App
{
    /************************************
     * PUBLIC FUNCTIONS
     ************************************/
    Logger::Logger(Levels logLevel, const std::string& logFileName, bool writeToConsole) : m_logLevel{logLevel}, m_isWriteToFileEnabled{!logFileName.empty()}, m_isWriteToConsoleEnabled{writeToConsole}
    {
        if(m_isWriteToFileEnabled)
        {
            m_logFileHandle.open(logFileName, std::ios::app);
            if(!m_logFileHandle.is_open())
            {
                std::cerr << "Un-able to open the log file" << std::endl;
                m_isWriteToFileEnabled = false;
            }
        }
    }
    Logger::~Logger()
    {
        if(m_logFileHandle.is_open())
        {
            m_logFileHandle.close();
        }
    }
    void Logger::setWriteToFile(bool enabled, const std::string& fileName)
    {
        std::lock_guard<std::mutex> lock(m_logMutex);
        if(enabled && (!fileName.empty()))
        {
            if(m_logFileHandle.is_open())
            {
                m_logFileHandle.close();
            }
            m_logFileName = fileName;
            m_logFileHandle.open(m_logFileName, std::ios::app);
            m_isWriteToFileEnabled = m_logFileHandle.is_open();
        }
        else if(enabled && (fileName.empty()))
        {
            if(!(m_logFileHandle.is_open()))
            {
                m_logFileHandle.open(m_logFileName, std::ios::app);
                m_isWriteToFileEnabled = m_logFileHandle.is_open();
            }
        }
        else
        {
            m_isWriteToFileEnabled = false;
            if(m_logFileHandle.is_open())
            {
                m_logFileHandle.close();
            }
        }
    }
    
    void Logger::printBuffer(void)
    {
        std::lock_guard<std::mutex> lock(m_logMutex);
        auto bufferIterator = m_buffer.begin();
        std::cout << "\n=== LOG BUFFER CONTENTS ===" << std::endl;
        for (const auto& entry : m_buffer)
        {
            std::cout << entry << std::endl;
        }
        std::cout << "=== END LOG BUFFER ===" << std::endl;
    }

    void Logger::dumpLogBufferToLogFile(const std::string& fileName)
    {
        std::lock_guard<std::mutex> lock(m_logMutex);
        std::ofstream fileHandle(fileName, std::ios::app);
        if(fileHandle.is_open())
        {
            for(const auto& iterator : m_buffer)
            {
                fileHandle << iterator << std::endl;
            }
            fileHandle.close();
            std::cout << "Logs dumped to: " << fileName << std::endl;
        }
        else
        {
            std::cerr << "Failed to open file for dumping: " << fileName << std::endl;
        }
    }

    void Logger::printStatistics(void) 
    {
        std::lock_guard<std::mutex> lock(m_logMutex);
        std::cout << "\n=== LOGGER STATISTICS ===" << std::endl;
        std::cout << "Buffer size: " << m_buffer.size() << " entries" << std::endl;
        std::cout << "Min log level: " << convertLevelToString(m_logLevel) << std::endl;
        std::cout << "Console output: " << (m_isWriteToConsoleEnabled ? "Enabled" : "Disabled") << std::endl;
        std::cout << "File output: " << (m_isWriteToFileEnabled ? "Enabled" : "Disabled") << std::endl;
        if (m_isWriteToFileEnabled)
        {
            std::cout << "Log file: " << m_logFileName << std::endl;
        }
        std::cout << "=========================" << std::endl;
    }
    /************************************
     * PRIVATE FUNCTIONS
     ************************************/
    /**
     * @brief helper function to convert enum value to string
     */
    std::string Logger::convertLevelToString(Levels level)
    {
        switch(level)
        {
            case Levels::DEBUG:    return "DEBUG";
            case Levels::INFO:     return "INFO";
            case Levels::WARNING:  return "WARNING";
            case Levels::ERROR:    return "ERROR";
            case Levels::CRITICAL: return "CRITICAL";
            default:               return "UNKNOWN";
        }
    }

    std::string Logger::getCurrentTimestamp(void) const
    {
        auto now = std::chrono::system_clock::now();
        auto timeNow = std::chrono::system_clock::to_time_t(now);
        std::stringstream stringStream;
        stringStream << std::put_time(std::localtime(&timeNow), "%Y-%m-%d %H:%M:%S");
        return stringStream.str();
    }

    std::string Logger::formatMessage(Levels level, const std::string& message)
    {
        std::stringstream stringStream;
        stringStream << "[" << getCurrentTimestamp() << "]" << "[" << convertLevelToString(level) << "] " << message;
        return stringStream.str();
    }

    void Logger::logMessage(Levels level, const std::string& message)
    {
        /* Filter out messages below the current log level */
        if(level < m_logLevel)
        {
            return;
        }
        else
        {
            std::lock_guard<std::mutex> lock(m_logMutex);
            std::string formattedMessage = formatMessage(level, message);
            m_buffer.push_back(formattedMessage);
            if(m_isWriteToConsoleEnabled)
            {
                std::cout << formattedMessage << std::endl;
            }
            else
            {
                /* Do nothing */
            }
            if(m_isWriteToFileEnabled && m_logFileHandle.is_open())
            {
                m_logFileHandle << formattedMessage << std::endl;
                m_logFileHandle.flush();
            }
            else
            {
                /* Do nothing */
            }
        }
    }
}
