#include "Epoll.h"

inline void Epoll::addFd(int fd, int type) {
    ev.events = EPOLLIN;
    ev.data.fd = fd;
    error(epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev),"epoll_ctl: add fd error");//将服务器fd添加到epoll树上。
}

std::vector<struct epoll_event> Epoll::poll() {
    epoll_wait(epollfd, events, MAX_NUM, -1);
    std::vector<struct epoll_event> v;
    for (auto it : events)
    {
        v.push_back(it);
    }
    return v;
}

Epoll::Epoll() {
    int epollfd = epoll_create1(0);
    bzero(&events, sizeof(events));
    bzero(&ev, sizeof(ev));
    error(epollfd, "epoll_create1");
}

Epoll::~Epoll(){
}