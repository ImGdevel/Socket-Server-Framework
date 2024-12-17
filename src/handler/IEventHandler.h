#ifndef IEVENTHANDLER_H
#define IEVENTHANDLER_H

#include "../common/HandlerTypes.h"
#include <unordered_map>

class IEventHandler {
public:
    virtual std::unordered_map<std::string, HandlerFunc> createHandlers() const = 0;
};

#endif
