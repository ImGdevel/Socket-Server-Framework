#ifndef JSONPARSER_H
#define JSONPARSER_H

#include <string>
#include <utility>
#include <nlohmann/json.hpp>
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

#endif