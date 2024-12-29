#ifndef REDIS_DB_H
#define REDIS_DB_H

#include "../interface/DBInterface.h"
#include <string>
#include <vector>

class redisContext;

class RedisDB : public DBInterface {
private:
    redisContext* context;

public:
    ~RedisDB();
    void connect(const std::string& host, const std::string& port, const std::string& unused1, const std::string& unused2) override;
    void execute(const std::string& query) override;
    std::vector<std::vector<std::string>> fetch(const std::string& query) override;
};

#endif