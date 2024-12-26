#ifndef PROTOBUFMESSAGE_H
#define PROTOBUFMESSAGE_H

#include "IMessage.h"

class ProtobufMessage : public IMessage {
private:
    std::string type;
    std::string content;

public:
    ProtobufMessage(const std::string& type, const std::string& content)
        : type(type), content(content) {}

    std::string getType() const override {
        return type;
    }

    std::string serialize() const override {
        return content;
    }
};

#endif
