#ifndef PROTOBUFPARSER_H
#define PROTOBUFPARSER_H

#include "IParser.h"
#include "ProtobufMessage.h"
#include <google/protobuf/message.h>

template <typename T>
class ProtobufParser : public IParser {
public:
    std::unique_ptr<IMessage> parse(const std::string& message) const override {
        T protoMessage;
        if (!protoMessage.ParseFromString(message)) {
            return nullptr;
        }

        std::string type = protoMessage.type();
        std::string content = protoMessage.content();

        return std::make_unique<ProtobufMessage>(type, content);
    }
};

#endif
