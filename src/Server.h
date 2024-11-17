#ifndef SERVER_H
#define SERVER_H

class Reactor;
class ThreadPool;

class Server {
public:
    Server(int port, int workerCount);
    ~Server();
    void run();
    
private:
    int port;
    int workerCount;
    bool isRunning;

    Reactor* reactor;
    ThreadPool* threadPool;
};

#endif
