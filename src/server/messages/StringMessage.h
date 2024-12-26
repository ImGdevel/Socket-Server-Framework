#ifndef STRINGMESSAGE_H
#define STRINGMESSAGE_H

#include "IMessage.h"

class StringMessage : public IMessage {
private:
    std::string type;
    std::string content;

public:
    StringMessage(const std::string& type, const std::string& content)
        : type(type), content(content) {}

    std::string getType() const override {
        return type;
    }

    std::string serialize() const override {
        return content;
    }
};

#endif
