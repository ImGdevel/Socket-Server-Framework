#ifndef IMESSAGE_H
#define IMESSAGE_H

#include <string>

class IMessage {
public:
    virtual ~IMessage() = default;
    virtual std::string getType() const = 0;
    virtual std::string serialize() const = 0;
};

#endif