#ifndef ClientRequest_H
#define ClientRequest_H

#include <memory>
#include "../session/ClientSession.h"
#include "../messages/IMessage.h"

class ClientRequest {
public:
    ClientRequest(const std::shared_ptr<ClientSession>& session, std::unique_ptr<IMessage> message)
        : session(session)
        , message(std::move(message)) {}

    const std::shared_ptr<ClientSession>& getSession() const { return session; }
    const std::unique_ptr<IMessage>& getMessage() const { return message; }

private:
    std::shared_ptr<ClientSession> session;
    std::unique_ptr<IMessage> message;
};

#endif 