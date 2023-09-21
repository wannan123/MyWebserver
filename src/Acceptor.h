#pragma once
#include "Socket.h"
#include "Eventloop.h"
#include <functional>
class Eventloop;
class Socket;
class InetAddress;
class Channel;
class Acceptor
{
    private:
        Eventloop *loop;
        Socket *sock;
        InetAddress *addr;
        Channel *acceptChannel;
        std::function<void(Socket *)> newConnections;
    public:
        Acceptor(Eventloop *_loop);
        ~Acceptor();
        void acceptConnection();
        void setNewConnectionCallback(std::function<void(Socket *)>);
};


