#ifndef IPARSER_H
#define IPARSER_H

#include <string>
#include <memory>
#include "IMessage.h"

class IParser {
public:
    virtual ~IParser() = default;
    virtual std::unique_ptr<IMessage> parse(const std::string& message) const = 0;
};

#endif