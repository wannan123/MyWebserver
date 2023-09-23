#pragma once
#include "Socket.h"
#include "Eventloop.h"
#include <functional>
class Eventloop;
class Socket;
class Channel;
class Connection
{
    private:
        Eventloop *loop;
        Socket *sock;
        Channel *handleChannel;
        std::function<void(Socket *)> deleteConnections;
    public:
        Connection(Eventloop *_loop, Socket *_sock);
        ~Connection();
        void echo(int fd);
        void setCallback(std::function<void(Socket *)> cb);
};

