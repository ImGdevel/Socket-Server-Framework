#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <string>
#include <cstring>
#include <cerrno>

enum LogLevel { DEBUG, INFO, WARNING, ERROR };

class Logger {
private:
    static LogLevel logLevel;

public:
    static void setLogLevel(LogLevel level) {
        logLevel = level;
    }

    static void debug(const std::string& message) {
        if (logLevel <= DEBUG) {
            std::cout << "[DEBUG] " << message << std::endl;
        }
    }

    static void info(const std::string& message) {
        if (logLevel <= INFO) {
            std::cout << "[INFO] " << message << std::endl;
        }
    }

    static void warning(const std::string& message) {
        if (logLevel <= WARNING) {
            std::cerr << "[WARNING] " << message << std::endl;
        }
    }

    static void error(const std::string& message) {
        if (logLevel <= ERROR) {
            std::cerr << "[ERROR] " << message << std::endl;
        }
    }
};

LogLevel Logger::logLevel = DEBUG;

#endif
