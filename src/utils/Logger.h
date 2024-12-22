#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>
#include <mutex>
#include <sys/stat.h>
#include <thread>
#include <queue>
#include <condition_variable>
#include <atomic>
#include <iomanip>

enum LogLevel { DEBUG, INFO, WARNING, ERROR };

class Logger {
private:
    inline static LogLevel logLevel = DEBUG;
    inline static LogLevel fileLogLevel = INFO;
    inline static bool writeToFile = true;
    inline static std::string logPath = "./log";
    inline static std::ofstream logFile;
    inline static std::mutex logMutex;
    inline static std::queue<std::string> logQueue;
    inline static std::condition_variable logCondition;
    inline static std::atomic<bool> running{true};
    inline static std::thread logThread;
    inline static size_t maxLogFileSize = 10 * 1024 * 1024;
    inline static int fileIndex = 1;

    class LoggerInitializer {
    public:
        LoggerInitializer() {
            Logger::initializeLogFile();
            Logger::startLoggingThread();
        }
    };

    inline static LoggerInitializer initializer;

    static void initializeLogFile() {
        if (writeToFile) {
            struct stat info;
            if (stat(logPath.c_str(), &info) != 0) {
                mkdir(logPath.c_str(), 0755);
            }

            // 날짜 기반 폴더 생성
            time_t now = time(nullptr);
            struct tm localTime;
            localtime_r(&now, &localTime);
            char dateStr[15];
            strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", &localTime);
            std::string folderPath = logPath + "/" + dateStr;

            struct stat folderInfo;
            if (stat(folderPath.c_str(), &folderInfo) != 0) {
                mkdir(folderPath.c_str(), 0755);  // 폴더가 없으면 생성
            }

            // 로그 파일 이름 생성 (날짜 + 식별번호)
            std::ostringstream fileNameStream;
            fileNameStream << folderPath << "/" << dateStr << "-" 
                           << std::setw(6) << std::setfill('0') << fileIndex << ".log";

            std::string fullFilePath = fileNameStream.str();
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
                logQueue.push(logStream.str());
                logCondition.notify_one();
            }
        }
    }

    static void startLoggingThread() {
        logThread = std::thread([](){
            while (running) {
                std::string logMessage;
                {
                    std::unique_lock<std::mutex> lock(logMutex);
                    logCondition.wait(lock, [](){ return !logQueue.empty() || !running; });

                    if (!logQueue.empty()) {
                        logMessage = logQueue.front();
                        logQueue.pop();
                    }
                }

                if (!logMessage.empty() && writeToFile && logFile.is_open()) {
                    logFile << logMessage << std::endl;
                    logFile.flush();
                    checkLogFileSize();
                }
            }
        });
    }

    static void checkLogFileSize() {
        if (static_cast<size_t>(logFile.tellp()) > maxLogFileSize) {
            logFile.close();
            ++fileIndex;
            initializeLogFile();
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

    static void setMaxLogFileSize(size_t size) {
        maxLogFileSize = size;
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
        running = false;
        logCondition.notify_all();
        if (logThread.joinable()) {
            logThread.join();
        }
        if (logFile.is_open()) {
            logFile.close();
        }
    }
};

#endif
