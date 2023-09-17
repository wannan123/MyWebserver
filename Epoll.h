#pragma once
#include <sys/epoll.h>
#include <vector>
#include <strings.h>
#include "util.h"
#include "Channel.h"
#define MAX_NUM 10
class Channel;
class Epoll
{
    private:
        int epollfd;
        struct epoll_event ev;
        struct epoll_event *events;
    public:
        void addFd(int fd, uint32_t op);
        std::vector<Channel*> poll();
        void updateChannel(Channel * Channel);
        void clear();
        Epoll();
        ~Epoll();
};


