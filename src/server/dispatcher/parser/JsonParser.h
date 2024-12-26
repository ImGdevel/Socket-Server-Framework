#ifndef JSONPARSER_H
#define JSONPARSER_H

#include <string>
#include <utility>
#include <nlohmann/json.hpp>
#include <rapidjson/document.h>
#include "IParser.h"

using json = nlohmann::json;

class JSONParser : public IParser {
public:
    std::pair<std::string, std::string> parse(const std::string& message) const override {
        try {
            auto jsonObj = json::parse(message);
            std::string type = jsonObj.at("type");
            std::string content = jsonObj.at("content");
            return {type, content};
        } catch (const std::exception& e) {
            return {"", ""};
        }
    }
};

class JSONParserRapid : public IParser {
public:
    std::pair<std::string, std::string> parse(const std::string& message) const override {
        rapidjson::Document document;
        if (document.Parse(message.c_str()).HasParseError()) {
            return {"", ""};
        }

        try {
            if (document.HasMember("type") && document["type"].IsString() &&
                document.HasMember("content") && document["content"].IsString()) {
                std::string type = document["type"].GetString();
                std::string content = document["content"].GetString();
                return {type, content};
            } else {
                return {"", ""};
            }
        } catch (...) {
            return {"", ""};
        }
    }
};

#endif
