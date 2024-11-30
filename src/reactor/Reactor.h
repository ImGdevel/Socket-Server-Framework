#ifndef REACTOR_H
#define REACTOR_H

#include <vector>
#include <functional>

class Reactor {
public:
    explicit Reactor(int port);
    ~Reactor();

    void start();

private:
    int port;
    int serverSocket;

    void acceptConnection();
};

#endif
