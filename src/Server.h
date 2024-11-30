#ifndef SERVER_H
#define SERVER_H

#include "reactor/Reactor.h"

class Server {
public:
    //싱글톤
    static Server* getInstance(int port, int workerCount);

    ~Server();

    void run();
    void terminate();

private:
    static Server* instance;
    
    int port;
    int workerCount;

    Reactor* reactor;
    
    Server(int port, int workerCount);

};

#endif 