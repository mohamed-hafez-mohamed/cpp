/**
 ********************************************************************************
 * @file    Logger.hpp
 * @author  MHafez
 * @date    26 August 2025
 * @brief   This file headers for the Logger class interfaces
 ********************************************************************************
 */

#ifndef _LOGGER_HH_
#define _LOGGER_HH_


/************************************
 * INCLUDES
 ************************************/
#include <stdint.h>
#include <string>
#include <vector>
#include <fstream>
#include <mutex>
#include <memory>
/************************************
 * NAMESPACES
 ************************************/

/**
 * @namespace App
 * @brief A collection of various application utilities.
 */
namespace App
{
    /**
     * @class   Logger
     * @brief   A thread-safe logging utility.
     *
     * This class provides a logging mechanism that is safe for use in
     * a multi-threaded environment. It supports logging messages to
     * both console and file outputs.
     */
    class Logger
    {
        public:
            /**
             * @brief enum class Levels is a local type represents log levels
             */
            enum class Levels : uint8_t 
            {
                DEBUG    = UINT8_C(0),
                INFO     = UINT8_C(1),
                WARNING  = UINT8_C(2),
                ERROR    = UINT8_C(3),
                CRITICAL = UINT8_C(4)
            };
            /**
             * @brief Constructs a Logger instance.
             * @param logLevel The logging level.
             * @param logFileName The name of the log file.
             * @param writeToConsole Flag indicating whether to write to console.
             */
            Logger(Levels logLevel, const std::string& logFileName, bool writeToConsole);
            /**
             * @brief Destroys the Logger instance.
             */
            virtual ~Logger();
            /**
             * @brief Log debug level.
             */
            void debug(const std::string& message)
            {
                logMessage(Levels::DEBUG, message);
            }
            /**
             * @brief Log info level.
             */
            void info(const std::string& message)
            {
                logMessage(Levels::INFO, message);
            }
            /**
             * @brief Log warning level.
             */
            void warning(const std::string& message)
            {
                logMessage(Levels::WARNING, message);
            }
            /**
             * @brief Log error level.
             */
            void error(const std::string& message)
            {
                logMessage(Levels::ERROR, message);
            }
            /**
             * @brief Log critical level.
             */
            void critical(const std::string& message)
            {
                logMessage(Levels::CRITICAL, message);
            }
            /**
             * @brief Set log level by user.
             */
            inline void setLogLevel(Levels logLevel)
            {
                std::lock_guard<std::mutex> lock(m_logMutex);
                m_logLevel = logLevel;
            }
            /**
             * @brief Get log level.
             */
            inline Levels getLogLevel() const
            {
                return m_logLevel;
            }
            /**
             * @brief Set write to console parameter.
             */
            inline void setWriteToConsole(bool enabled)
            {
                std::lock_guard<std::mutex> lock(m_logMutex);
                m_isWriteToConsoleEnabled = enabled;
            }
            /**
             * @brief Get log buffer.
             */
            inline std::vector<std::string> getLogBuffer() const
            {
                std::lock_guard<std::mutex> lock(m_logMutex);
                return m_buffer;
            }
            /**
             * @brief Get log buffer size.
             */
            inline size_t getBufferSize(void) const
            {
                std::lock_guard<std::mutex> lock(m_logMutex);
                return m_buffer.size();
            }
            /**
             * @brief Clear log buffer.
             */
            inline void clearLogBuffer(void)
            {
                std::lock_guard<std::mutex> lock(m_logMutex);
                m_buffer.clear();
                std::cout << "Log buffer cleared." << std::endl;
            }
            /**
             * @brief Flush log buffer.
             */
            inline void flushLogfile(void)
            {
                std::lock_guard<std::mutex> lock(m_logMutex);
                if(m_logFileHandle.is_open())
                {
                    m_logFileHandle.flush();
                }
                
            }
            /**
             * @brief Set write to file parameter.
             */
            void setWriteToFile(bool enabled, const std::string& fileName = "");
            /**
             * @brief Print Log Buffer.
             */
            void printBuffer(void);
            /**
             * @brief Dump log buffer into a file.
             */
            void dumpLogBufferToLogFile(const std::string& fileName = "");
            /**
             * @brief Print some statistics.
             */
            void printStatistics(void);
        private:
            Levels m_logLevel;
            std::vector<std::string> m_buffer;
            mutable std::mutex m_logMutex;
            std::string m_logFileName;
            std::ofstream m_logFileHandle;
            bool m_isWriteToFileEnabled;
            bool m_isWriteToConsoleEnabled;
            /**
             * @brief helper function to convert enum value to string
             */
            std::string convertLevelToString(Levels level);
            /**
             * @brief helper function to get time stamp
             */
            std::string getCurrentTimestamp(void) const;
            /**
             * @brief helper function to format the log message
             */
            std::string formatMessage(Levels level, const std::string& message);
            /**
             * @brief helper function to log the message into the buffer
             */
            void logMessage(Levels level, const std::string& message);
    };
     /**
      * @brief Singleton logger for global access
      */
    class GlobalLogger
    {
        public:
            static Logger& getInstance()
            {
                std::lock_guard<std::mutex> lock(instanceMutex);
                if (!instance)
                {
                    instance = std::make_unique<Logger>(Logger::Levels::DEBUG, "application.log", true);
                }
                return *instance;
            }
        
            static void setInstance(std::unique_ptr<Logger> logger)
            {
                std::lock_guard<std::mutex> lock(instanceMutex);
                instance = std::move(logger);
            }
        private:
            static std::unique_ptr<Logger> instance;
            static std::mutex instanceMutex;
    };
}

// Convenience macros for global logger
#define LOG_DEBUG(msg)    App::GlobalLogger::getInstance().debug(msg)
#define LOG_INFO(msg)     App::GlobalLogger::getInstance().info(msg)
#define LOG_WARNING(msg)  App::GlobalLogger::getInstance().warning(msg)
#define LOG_ERROR(msg)    App::GlobalLogger::getInstance().error(msg)
#define LOG_CRITICAL(msg) App::GlobalLogger::getInstance().critical(msg)

#endif // _LOGGER_HH_