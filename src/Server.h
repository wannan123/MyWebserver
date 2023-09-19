#pragma once
#include <functional>
#include "Eventloop.h"
#include "Socket.h"
class Server
{
    private:
        Eventloop *ep;
    public:
        Server(Eventloop*);
        ~Server();
        void handleReadEvent(int);
        void newConnection(Socket *serv_sock);
};
