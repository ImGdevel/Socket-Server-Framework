#ifndef JSONPARSER_H
#define JSONPARSER_H

#include "IParser.h"
#include "JsonMessage.h"
#include "rapidjson/document.h"
#include <nlohmann/json.hpp>
#include <utility>

using json = nlohmann::json;

class JSONParser : public IParser {
public:
    std::unique_ptr<IMessage> parse(const std::string& message) const override {
        try {
            auto jsonObj = json::parse(message);

            if (jsonObj.contains("type") && jsonObj.contains("content")) {
                std::string type = jsonObj.at("type").get<std::string>();
                auto content = jsonObj.at("content");

                return std::make_unique<JSONMessage>(type, content);
            }
        } catch (const std::exception& e) {
            
        }

        return nullptr;
    }
};


class JSONParserRapid : public IParser {
public:
    std::unique_ptr<IMessage> parse(const std::string& message) const override {
        rapidjson::Document document;
        if (document.Parse(message.c_str()).HasParseError()) {
            return nullptr;
        }

        if (document.HasMember("type") && document["type"].IsString() &&
            document.HasMember("content") && document["content"].IsObject()) {

            std::string type = document["type"].GetString();
            rapidjson::Document content;
            content.CopyFrom(document["content"], content.GetAllocator());

            return std::make_unique<JSONMessageRapid>(type, content);
        }

        return nullptr;
    }
};

#endif
