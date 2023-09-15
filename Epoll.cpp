#include "Epoll.h"
#include <unistd.h>
#include <iostream>
using namespace std;
void Epoll::addFd(int fd, uint32_t type) {
    //struct epoll_event ev;
    bzero(&ev, sizeof(ev));
    ev.events = type;
    ev.data.fd = fd;
    //error(epollfd,"1223");
    error(epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev),"epoll_ctl: add fd error");//将服务器fd添加到epoll树上。
    
}

std::vector<struct epoll_event> Epoll::poll() {
    // int nfds = epoll_wait(epollfd, events, MAX_NUM, -1);
    // error(nfds,"epoll_wait error");
    // std::vector<struct epoll_event> v;
    // for(int i = 0; i < nfds; ++i){
    //     v.push_back(events[i]);
    // }
    // return v;
    std::vector<epoll_event> activeEvents;
    // cout<<"9"<<endl;
    int nfds = epoll_wait(epollfd, events, MAX_NUM, -1);
    error(nfds, "epoll wait error");
    for(int i = 0; i < nfds; ++i){
        activeEvents.push_back(events[i]);
    }
    return activeEvents;
}

void Epoll::clear() {
    bzero(events, sizeof(*events) * MAX_NUM);
    bzero(&ev, sizeof(ev));
}

Epoll::Epoll():epollfd(-1), events(nullptr) {

    epollfd = epoll_create1(0);
    error(epollfd, "epoll_create1");
    events = new epoll_event[MAX_NUM];
    bzero(&ev, sizeof(ev));
    bzero(events, sizeof(*events) * MAX_NUM);//这里第一个参数events是个指针，如果用&event的话就是个二级指针了。
    
    
    
    // epollfd = epoll_create1(0);
    // error(epollfd == -1, "epoll create error");
    // events = new epoll_event[MAX_NUM];
    // bzero(&ev, sizeof(ev));
    // bzero(events, sizeof(*events) * MAX_NUM);
}







