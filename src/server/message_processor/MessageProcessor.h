#ifndef MESSAGE_PROCESSOR_H
#define MESSAGE_PROCESSOR_H

#include "MessageDispatcher.h"
#include "FilterChain.h"
#include "IParser.h"
#include "ClientSession.h"
#include "Logger.h"
#include <memory>

class MessageProcessor {
public:
    MessageProcessor(std::unique_ptr<MessageDispatcher> dispatcher, std::unique_ptr<FilterChain> filterChain, std::unique_ptr<IParser> parser);
    void processMessage(std::shared_ptr<ClientSession> session, const std::string& message);

private:
    std::unique_ptr<MessageDispatcher> dispatcher;
    std::unique_ptr<FilterChain> filterChain;
    std::unique_ptr<IParser> parser;
};

#endif 