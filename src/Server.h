#pragma once
#include <functional>
#include "Eventloop.h"
#include "Acceptor.h"
#include "Socket.h"
class Server
{
    private:
        Eventloop *ep;
        Acceptor *acceptor;
    public:
        Server(Eventloop*);
        ~Server();
        void handleReadEvent(int);
        void newConnection(Socket *serv_sock);
};
