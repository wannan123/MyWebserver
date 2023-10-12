#pragma once
#include "Acceptor.h"
#include "Connection.h"
#include "Eventloop.h"
#include "Socket.h"
#include "ThreadPool.h"
#include <functional>
#include <map>
#include <vector>
class Server {
private:
    Eventloop *acceptReactor;
    Acceptor *acceptor;
    std::vector<Eventloop *> subReactors;
    std::map<int, Connection *> connections;
    ThreadPool *threads;

public:
    Server(Eventloop *);
    ~Server();
    // void handleReadEvent(int);
    void newConnection(Socket *serv_sock);
    void deleteConnection(int sockfd);
};
