#pragma once
#include <functional>
#include <map>
#include <vector>
#include "Connection.h"
#include "Eventloop.h"
#include "Acceptor.h"
#include "Socket.h"
#include "ThreadPool.h"
class Server
{
    private:
        Eventloop *acceptReactor;
        Acceptor *acceptor;
        std::vector<Eventloop *> subReactors;
        std::map<int, Connection *> connections;
        ThreadPool *threads;
    public:
        Server(Eventloop*);
        ~Server();
        //void handleReadEvent(int);
        void newConnection(Socket *serv_sock);
        void deleteConnection(int sockfd);
};
