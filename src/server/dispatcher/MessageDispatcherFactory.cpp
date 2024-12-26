#include "MessageDispatcherFactory.h"
#include "../handler/TestJSONEventHandler.h"
#include "../messages/parser/JsonParser.h"
#include <memory>
#include <stdexcept>
#include "Logger.h"

using namespace std;

unique_ptr<MessageDispatcher> MessageDispatcherFactory::createDispatcher(const string& format) {
    unique_ptr<MessageDispatcher> dispatcher = nullptr;

    if(format == "json"){
        dispatcher = createJSONDispatcher();
    }
    else if(format == "json-rapid"){
        dispatcher = createJSONRapidDispatcher();
    }else{
        throw runtime_error("Failed to bind socket");
    }

    return dispatcher;
}

unique_ptr<MessageDispatcher> MessageDispatcherFactory::createJSONDispatcher() {
    auto parser = make_unique<JSONParser>();
    auto dispatcher = make_unique<MessageDispatcher>(move(parser));

    TestJSONEventHandler handler;
    HandlerConfigurator::registerHandlers(*dispatcher, handler);

    return dispatcher;
}

unique_ptr<MessageDispatcher> MessageDispatcherFactory::createJSONRapidDispatcher() {
    auto parser = make_unique<JSONParserRapid>();
    auto dispatcher = make_unique<MessageDispatcher>(move(parser));

    TestJSONEventHandler handler;
    HandlerConfigurator::registerHandlers(*dispatcher, handler);

    return dispatcher;
}