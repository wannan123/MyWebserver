#pragma once
#include "Buffer.h"
#include "Eventloop.h"
#include "Socket.h"
#include <functional>
class Eventloop;
class Socket;
class Channel;
class Connection {
private:
    Eventloop *loop;
    Socket *sock;
    Channel *handleChannel;
    std::function<void(int)> deleteConnectionCallback;
    Buffer *readBuffer;

public:
    Connection(Eventloop *_loop, Socket *_sock);
    ~Connection();
    void echo(int fd);
    // void setCallback(std::function<void(Socket *)> cb);
    void send(int sockfd);
    void setDeleteConnectionCallback(std::function<void(int)> _cb);
};
