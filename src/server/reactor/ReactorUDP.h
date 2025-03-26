#ifndef REACTOR_UDP_H
#define REACTOR_UDP_H

#include "Reactor.h"
#include "ClientSessionManagerUDP.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>

class ReactorUDP : public Reactor {
public:
    explicit ReactorUDP(int port, ThreadPool& threadPool, MessageProcessor& messageProcessor);
    ~ReactorUDP();

    void start() override;
    void stop() override;

private:
    int serverSocket;
    bool running;
    ClientSessionManagerUDP sessionManager;
    static constexpr int BUFFER_SIZE = 1024;

    void setupServerSocket();
    void receiveLoop();
    void handleIncomingMessage(const sockaddr_in& clientAddr, const std::string& message);
};

#endif