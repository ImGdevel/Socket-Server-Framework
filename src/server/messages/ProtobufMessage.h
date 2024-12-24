#ifndef PROTOBUFMESSAGE_H
#define PROTOBUFMESSAGE_H

#include "IMessage.h"
#include <google/protobuf/message.h>

template <typename T>
class ProtobufMessage : public IMessage {
private:
    std::string type;
    T content;

public:
    ProtobufMessage(const std::string& type, const T& content)
        : type(type), content(content) {}

    std::string getType() const override {
        return type;
    }

    std::string serialize() const override {
        std::string serialized;
        content.SerializeToString(&serialized);
        return serialized;
    }
};

#endif
