#ifndef PROTOBUFPARASER_H
#define PROTOBUFPARASER_H

#include "IParser.h"
#include <google/protobuf/message.h>

template <typename T>
class ProtobufParser : public IParser {
public:
    std::pair<std::string, std::string> parse(const std::string& message) const override {
        T protoMessage;
        if (!protoMessage.ParseFromString(message)) {
            return {"", ""};
        }

        std::string type = protoMessage.type();
        std::string content = protoMessage.content();
        return {type, content};
    }
};

#endif