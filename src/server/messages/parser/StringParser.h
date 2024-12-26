#ifndef STRINGPARSER_H
#define STRINGPARSER_H

#include "IParser.h"
#include "StringMessage.h"

class StringParser : public IParser {
public:
    std::unique_ptr<IMessage> parse(const std::string& message) const override {
        auto delimiterPos = message.find(":");
        if (delimiterPos == std::string::npos) {
            return nullptr;
        }

        std::string type = message.substr(0, delimiterPos);
        std::string content = message.substr(delimiterPos + 1);

        return std::make_unique<StringMessage>(type, content);
    }
};

#endif
