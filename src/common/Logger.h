#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <string>
#include <cstring>
#include <cerrno>

enum LogLevel { DEBUG ,INFO, WARNING, ERROR };

class Logger {
public:

    static void debug(const std::string& message) {
        std::cout << "[DEBUG] " << message << std::endl;
    }

    static void info(const std::string& message) {
        std::cout << "[INFO] " << message << std::endl;
    }

    static void warning(const std::string& message) {
        std::cerr << "[WARNING] " << message << std::endl;
    }

    static void error(const std::string& message) {
        std::cerr << "[ERROR] " << message << std::endl;
    }
};

#endif
