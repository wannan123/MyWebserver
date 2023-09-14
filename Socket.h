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
        int accept(InetAddress *fd);
        void setnonblocking(int clnt_sockfd);
        void do_use_fd(int fd);
        int getFd();
        Socket();
        ~Socket();
};





