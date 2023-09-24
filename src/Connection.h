#pragma once
#include "Socket.h"
#include "Eventloop.h"
#include <functional>
#include "Buffer.h"
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
        Buffer *readBuffer;

    public:
        Connection(Eventloop *_loop, Socket *_sock);
        ~Connection();
        void echo(int fd);
        void setCallback(std::function<void(Socket *)> cb);
};

