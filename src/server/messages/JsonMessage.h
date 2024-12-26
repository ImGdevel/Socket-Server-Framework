#ifndef JSONMESSAGE_H
#define JSONMESSAGE_H

#include "IMessage.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <nlohmann/json.hpp>

class JSONMessage : public IMessage {
private:
    std::string type;
    nlohmann::json content;

public:
    JSONMessage(const std::string& type, const nlohmann::json& content)
        : type(type), content(content) {}

    std::string getType() const override {
        return type;
    }

    std::string serialize() const override {
        return content.dump();
    }

    const nlohmann::json& getContent() const {
        return content;
    }
};

class JSONMessageRapid : public IMessage {
private:
    std::string type;
    rapidjson::Document content;

public:
    JSONMessageRapid(const std::string& type, const rapidjson::Document& content)
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

#endif