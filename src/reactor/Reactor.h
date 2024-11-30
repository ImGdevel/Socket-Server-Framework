#ifndef REACTOR_H
#define REACTOR_H

#include <vector>
#include <functional>

class Reactor {
public:
    explicit Reactor(int port);
    ~Reactor();

    void start();
    void stop();

private:
    int port;
    int serverSocket;

    bool running = false;
    
    void setupServerSocket();
    void acceptConnection();
    
};

#endif
