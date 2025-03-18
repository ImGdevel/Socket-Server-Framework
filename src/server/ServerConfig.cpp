#include "ServerConfig.h"
#include "Logger.h"
#include <cstdlib>
#include <string>

int ServerConfig::getEnvVarOrDefault(const char* envVar, int defaultValue) {
    const char* value = std::getenv(envVar);
    if (value && *value != '\0') {
        try {
            return std::stoi(value);
        } catch (const std::invalid_argument&) {
            Logger::warning("[Warning] Invalid integer format for " + std::string(envVar) + 
                            ". Using default: " + std::to_string(defaultValue));
        } catch (const std::out_of_range&) {
            Logger::warning("[Warning] Value out of range for " + std::string(envVar) + 
                            ". Using default: " + std::to_string(defaultValue));
        }
    }
    return defaultValue;
}

int ServerConfig::getPort() {
    return getEnvVarOrDefault("SSF_PORT", 8080);
}

int ServerConfig::getWorkerCount() {
    return getEnvVarOrDefault("SSF_WORKER_COUNT", 10);
}
