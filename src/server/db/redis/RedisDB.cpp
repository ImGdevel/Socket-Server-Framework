#include "RedisDB.h"
#include "Logger.h"
#include <hiredis/hiredis.h>
#include <stdexcept>
#include <iostream>

using namespace std;

RedisDB::~RedisDB() {
    try {
        if (context) {
            redisFree(context);
            Logger::info("Redis connection closed.");
        }
    } catch (const std::exception& e) {
        string errorMsg = "Unexpected error during Redis cleanup: " + std::string(e.what());
        throw runtime_error("Redis connection failed: " + errorMsg);
    }
    context = nullptr;
}


void RedisDB::connect(const string& host, const string& port, const string&, const string&) {
    Logger::debug("Attempting to connect to Redis server at " + host + ":" + port);
    context = redisConnect(host.c_str(), stoi(port));
    if (context == nullptr || context->err) {
        string errorMsg = "Redis Connection Error: " + string(context ? context->errstr : "Unknown error");
        throw runtime_error("Redis connection failed: " + errorMsg);
    }
    Logger::info("Redis Connection established to " + host + ":" + port);
}

void RedisDB::execute(const string& query) {
    Logger::debug("Executing Redis command: " + query);
    redisReply* reply = static_cast<redisReply*>(redisCommand(context, query.c_str()));
    if (!reply) {
        string errorMsg = "Redis Query Error: " + string(context->errstr);
        throw runtime_error("Redis query failed: " + errorMsg);
    }
    Logger::debug("Redis command executed successfully: " + query);
    freeReplyObject(reply);
}

vector<vector<string>> RedisDB::fetch(const string& query) {
    Logger::debug("Fetching data with Redis command: " + query);
    redisReply* reply = static_cast<redisReply*>(redisCommand(context, query.c_str()));
    vector<vector<string>> results;

    if (!reply) {
        string errorMsg = "Redis Query Error: " + string(context->errstr);
        throw runtime_error("Redis fetch failed: " + errorMsg);
    }

    if (reply->type == REDIS_REPLY_ARRAY) {
        for (size_t i = 0; i < reply->elements; ++i) {
            if (reply->element[i]->type == REDIS_REPLY_STRING) {
                results.push_back({reply->element[i]->str});
            } else {
                Logger::warning("Unexpected reply type in array: " + to_string(reply->element[i]->type));
            }
        }
        Logger::debug("Redis fetch successful. Rows returned: " + to_string(results.size()));
    } else {
        Logger::warning("Expected an array reply but received type: " + to_string(reply->type));
    }

    freeReplyObject(reply);
    return results;
}
