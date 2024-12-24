#ifndef JSONMESSAGE_H
#define JSONMESSAGE_H

#include "IMessage.h"
#include "rapidjson/document.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h" 

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
};

#endif
