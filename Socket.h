#pragma once
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <cstring>
#include <unistd.h>
#include "util.h"
#include "InetAddress.h"
#include "Epoll.h"
using namespace std;
class Socket
{
    private:
        int sockfd;
    public:
        Epoll *ep;
        void bind(InetAddress *address);
        void listen();
        void setnonblocking();
        int accept(InetAddress *fd);
        int getFd();
        Socket();
        ~Socket();
};





