#ifndef IPARSER_H
#define IPARSER_H

#include "IMessage.h"
#include <string>
#include <memory>

class IParser {
public:
    virtual ~IParser() = default;
    virtual std::unique_ptr<IMessage> parse(const std::string& message) const = 0;
};

#endif