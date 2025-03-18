#include "MessageDispatcherFactory.h"
#include "../handler/TestJSONEventHandler.h"
#include "../messages/parser/JsonParser.h"
#include <memory>
#include <stdexcept>
#include "Logger.h"

using namespace std;

unique_ptr<MessageDispatcher> MessageDispatcherFactory::createDispatcher(const string& format, IEventHandler& handler) {
    unique_ptr<MessageDispatcher> dispatcher = nullptr;

    if(format == "json"){
        dispatcher = createJSONDispatcher(handler);
    }
    else if(format == "json-rapid"){
        dispatcher = createJSONRapidDispatcher(handler);
    }else{
        throw runtime_error("Failed to bind socket");
    }

    return dispatcher;
}

unique_ptr<MessageDispatcher> MessageDispatcherFactory::createJSONDispatcher(IEventHandler& handler) {
    auto parser = make_unique<JSONParser>();
    auto dispatcher = make_unique<MessageDispatcher>(move(parser));

    HandlerConfigurator::registerHandlers(*dispatcher, handler);

    return dispatcher;
}

unique_ptr<MessageDispatcher> MessageDispatcherFactory::createJSONRapidDispatcher(IEventHandler& handler) {
    auto parser = make_unique<JSONParserRapid>();
    auto dispatcher = make_unique<MessageDispatcher>(move(parser));

    HandlerConfigurator::registerHandlers(*dispatcher, handler);

    return dispatcher;
}