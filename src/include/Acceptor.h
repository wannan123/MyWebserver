#pragma once
#include "Eventloop.h"
#include "Socket.h"
#include <functional>
class Eventloop;
class Socket;
class InetAddress;
class Channel;
class Acceptor {
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
