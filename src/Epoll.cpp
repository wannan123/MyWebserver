#include "include/Epoll.h"
#include <iostream>
#include <unistd.h>
// using namespace std;
// void Epoll::addFd(int fd, uint32_t op) {
//     struct epoll_event ev;
//     bzero(&ev, sizeof(ev));
//     ev.events = op;
//     ev.data.fd = fd;
//     error(epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev),"epoll_ctl: add fd
//     error");//将服务器fd添加到epoll树上。
// }

std::vector<Channel *> Epoll::poll() {
  std::vector<Channel *> activeEvents;
  int nfds = epoll_wait(epollfd, events, MAX_NUM, -1);
  error(nfds, "epoll wait error");
  for (int i = 0; i < nfds; ++i) {
    Channel *c = (Channel *)events[i].data.ptr;
    c->setRevents(events[i].events);
    activeEvents.push_back(c);
  }
  return activeEvents;
}

void Epoll::updateChannel(Channel *Channel) {
  int fd = Channel->getFd();
  struct epoll_event ev;
  bzero(&ev, sizeof(ev));

  ev.events = Channel->getEvent();
  ev.data.ptr = Channel;

  if (!Channel->getInEpoll()) {
    error(epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev),
          "epoll_ctl: add fd error");
    Channel->setEpoll();
  } else {
    error(epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &ev),
          "epoll_ctl: modify fd error");
  }
}

// void Epoll::clear() {
//     bzero(events, sizeof(*events) * MAX_NUM);
//     bzero(&ev, sizeof(ev));
// }

Epoll::Epoll() : epollfd(-1), events(nullptr) {

  epollfd = epoll_create1(0);
  error(epollfd, "epoll_create1");
  events = new epoll_event[MAX_NUM];
  // bzero(&ev, sizeof(ev));
  bzero(
      events,
      sizeof(*events) *
          MAX_NUM);  //这里第一个参数events是个指针，如果用&event的话就是个二级指针了。
}

Epoll::~Epoll() {
  if (epollfd != -1) {
    close(epollfd);
    epollfd = -1;
  }
  delete[] events;
}
