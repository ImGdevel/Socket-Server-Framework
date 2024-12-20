#ifndef SERVER_H
#define SERVER_H

#include "reactor/Reactor.h"
#include "threadpool/ThreadPool.h"

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

    std::unique_ptr<Reactor> reactor; 
    std::unique_ptr<ThreadPool> threadPool;
    MessageDispatcher messageDispatcher;
    
    Server(int port, int workerCount);
    Server(const Server&) = delete;
    Server& operator=(const Server&) = delete;

};

#endif 