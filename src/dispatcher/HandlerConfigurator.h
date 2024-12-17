#ifndef HANDLERCONFIGURATOR_H
#define HANDLERCONFIGURATOR_H

class MessageDispatcher;
class EventHandler;

class HandlerConfigurator {
public:
    static void registerHandlers(MessageDispatcher& dispatcher, const EventHandler& handler) ;
};

#endif
