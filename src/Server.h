#pragma once
#include <functional>
#include <map>
#include "Connection.h"
#include "Eventloop.h"
#include "Acceptor.h"
#include "Socket.h"
class Server
{
    private:
        Eventloop *ep;
        Acceptor *acceptor;
        std::map<int, Connection *> connections;
    public:
        Server(Eventloop*);
        ~Server();
        //void handleReadEvent(int);
        void newConnection(Socket *serv_sock);
        void deleteConnection(int sockfd);
};
