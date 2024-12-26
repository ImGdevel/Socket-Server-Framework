#ifndef JSONMESSAGE_H
#define JSONMESSAGE_H

#include "IMessage.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class JsonMessage : public IMessage {
private:
    std::string type;
    rapidjson::Document content;

public:
    JsonMessage(const std::string& type, const rapidjson::Document& content)
        : type(type) {
        this->content.CopyFrom(content, this->content.GetAllocator());
    }

    std::string getType() const override {
        return type;
    }

    std::string serialize() const override {
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        content.Accept(writer);
        return buffer.GetString();
    }

    const rapidjson::Document& getContent() const {
        return content;
    }
};

class JsonMessageNlohmann : public IMessage {
private:
    std::string type;
    json content;

public:
    JsonMessageNlohmann(const std::string& type, const json& content)
        : type(type), content(content) {}

    std::string getType() const override {
        return type;
    }

    std::string serialize() const override {
        return content.dump();
    }

    const json& getContent() const {
        return content;
    }
};

#endif