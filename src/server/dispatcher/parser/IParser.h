#ifndef IPARASER_H
#define IPARASER_H

#include <string>
#include <utility>

class IParser {
public:
    virtual ~IParser() = default;
    virtual std::pair<std::string, std::string> parse(const std::string& message) const = 0;
};

#endif