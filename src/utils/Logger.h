#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>
#include <mutex>
#include <sys/stat.h>

enum LogLevel { DEBUG, INFO, WARNING, ERROR };

class Logger {
private:
    inline static LogLevel logLevel = DEBUG;
    inline static LogLevel fileLogLevel = INFO;
    inline static bool writeToFile = true;
    inline static std::string logPath = "./log";
    inline static std::ofstream logFile;
    inline static std::mutex logMutex;

    // 로그 초기화 도우미 클래스
    class LoggerInitializer {
    public:
        LoggerInitializer() {
            Logger::initializeLogFile();
        }
    };

    // 정적 초기화 객체
    inline static LoggerInitializer initializer;

    // 로그 파일 초기화
    static void initializeLogFile() {
        if (writeToFile) {
            // 로그 디렉토리 생성
            struct stat info;
            if (stat(logPath.c_str(), &info) != 0) {
                mkdir(logPath.c_str(), 0755);
            }

            // 날짜 기반 파일 이름 생성
            time_t now = time(nullptr);
            struct tm localTime;
            localtime_r(&now, &localTime);
            char fileName[15];
            strftime(fileName, sizeof(fileName), "%Y-%m-%d.log", &localTime);

            // 파일 열기 (이어쓰기 모드)
            std::string fullFilePath = logPath + "/" + fileName;
            logFile.open(fullFilePath, std::ios::app);
            if (!logFile.is_open()) {
                std::cerr << "[ERROR] Failed to open log file: " << fullFilePath << std::endl;
                writeToFile = false;
            }
        }
    }

    static std::string getCurrentTime() {
        time_t now = time(nullptr);
        struct tm localTime;
        localtime_r(&now, &localTime);
        char buffer[20];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &localTime);
        return std::string(buffer);
    }

    static void logMessage(LogLevel level, const std::string& message) {
        static const char* levelStrings[] = { "DEBUG", "INFO", "WARNING", "ERROR" };
        std::string timeHeader = getCurrentTime();
        std::ostringstream logStream;
        logStream << timeHeader << " [" << levelStrings[level] << "] " << message;

        {
            std::lock_guard<std::mutex> lock(logMutex);
            if (logLevel <= level) {
                if (level == WARNING || level == ERROR)
                    std::cerr << logStream.str() << std::endl;
                else
                    std::cout << logStream.str() << std::endl;
            }

            if (writeToFile && fileLogLevel <= level && logFile.is_open()) {
                logFile << logStream.str() << std::endl;
                logFile.flush();
            }
        }
    }

public:
    static void setLogLevel(LogLevel level) {
        logLevel = level;
    }

    static void setFileLogLevel(LogLevel level) {
        fileLogLevel = level;
    }

    static void setWriteToFile(bool enable) {
        writeToFile = enable;
        if (writeToFile && !logFile.is_open()) {
            initializeLogFile();
        }
    }

    static void setLogPath(const std::string& path) {
        logPath = path;
        if (writeToFile) {
            logFile.close();
            initializeLogFile();
        }
    }

    static void debug(const std::string& message) {
        logMessage(DEBUG, message);
    }

    static void info(const std::string& message) {
        logMessage(INFO, message);
    }

    static void warning(const std::string& message) {
        logMessage(WARNING, message);
    }

    static void error(const std::string& message) {
        logMessage(ERROR, message);
    }

    static void close() {
        if (logFile.is_open()) {
            logFile.close();
        }
    }
};

#endif
