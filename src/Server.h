#ifndef SERVER_H
#define SERVER_H

class Server {
public:
    Server(int port, int workerCount);
    ~Server();

    void run();

private:
    int port;
    int workerCount;
};

#endif 