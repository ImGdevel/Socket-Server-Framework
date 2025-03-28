#ifndef PARSERFACTORY_H
#define PARSERFACTORY_H

#include "IParser.h"
#include "JsonParser.h"
#include "ProtobufParser.h"
#include "StringParser.h"
#include <functional>
#include <unordered_map>

class ParserFactory {
public:
    using ParserCreator = std::function<std::unique_ptr<IParser>()>;

    ParserFactory() {
        // 기본적으로 제공
        registerParser("json", []() { return std::make_unique<JSONParser>(); });
        registerParser("json-rapid", []() { return std::make_unique<JSONParserRapid>(); });
    }

    void registerParser(const std::string& type, ParserCreator creator) {
        parsers[type] = creator;
    }

    std::unique_ptr<IParser> createParser(const std::string& type) const {
        auto it = parsers.find(type);
        if (it != parsers.end()) {
            return it->second();
        }
        return nullptr;
    }

private:
    std::unordered_map<std::string, ParserCreator> parsers;
};

#endif
