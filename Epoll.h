#pragma once
#include <sys/epoll.h>
#include <vector>
#include <strings.h>
#include "util.h"
#define MAX_NUM 10
class Epoll
{
    private:
        int epollfd;
        struct epoll_event ev;
        struct epoll_event events[MAX_NUM];
    public:
        void addFd(int fd, int type);
        std::vector<struct epoll_event> poll();
        Epoll();
        ~Epoll();
};


