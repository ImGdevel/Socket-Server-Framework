#ifndef IPARASER_H
#define IPARASER_H

#include <string>
#include <utility>

class IParser {
public:
    virtual ~IParser() = default;
    virtual std::pair<std::string, std::string> parse(const std::string& message) const = 0;
};

class StringParser : public IParser {
public:
    std::pair<std::string, std::string> parse(const std::string& message) const override {
        auto delimiterPos = message.find(":");
        if (delimiterPos == std::string::npos) {
            return {"", ""};
        }
        std::string type = message.substr(0, delimiterPos);
        std::string content = message.substr(delimiterPos + 1);
        return {type, content};
    }
};

#endif