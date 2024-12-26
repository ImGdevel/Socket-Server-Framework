#ifndef MESSAGEDISPATCHERFACTORY_H
#define MESSAGEDISPATCHERFACTORY_H

#include "MessageDispatcher.h"
#include "HandlerConfigurator.h"
#include "memory"

class MessageDispatcherFactory {
public:
    static std::unique_ptr<MessageDispatcher> createDispatcher(const std::string& format);

private:

    static std::unique_ptr<MessageDispatcher> createJSONDispatcher();
    static std::unique_ptr<MessageDispatcher> createJSONRapidDispatcher();

};

#endif