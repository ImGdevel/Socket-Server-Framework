#ifndef HANDLERCONFIGURATOR_H
#define HANDLERCONFIGURATOR_H

class MessageDispatcher;
class IEventHandler;

class HandlerConfigurator {
public:
    static void registerHandlers(MessageDispatcher& dispatcher, const IEventHandler& handler) ;
};

#endif
