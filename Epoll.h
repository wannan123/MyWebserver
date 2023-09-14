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
        struct epoll_event *events;
    public:
        void addFd(int fd, uint32_t type);
        std::vector<struct epoll_event> poll();
        void clear();
        Epoll();
        ~Epoll();
};


