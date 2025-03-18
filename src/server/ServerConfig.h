#ifndef SERVER_CONFIG_H
#define SERVER_CONFIG_H


class ServerConfig {
public:
    static int getPort();
    static int getWorkerCount();

private:
    static int getEnvVarOrDefault(const char* envVar, int defaultValue);
};

#endif 