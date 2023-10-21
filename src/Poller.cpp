#include "include/Poller.h" 
#include <iostream>
#include <cstring>
#include <unistd.h>
#ifdef OS_LINUX
Poller::Poller() : pollerfd(-1), events(nullptr) {
  pollerfd = epoll_create1(0);
  error(pollerfd, "epoll_create1");
  events = new epoll_event[MAX_NUM];
  memset(events,0,sizeof(*events) *MAX_NUM); // 这里第一个参数events是个指针，如果用&event的话就是个二级指针了。
}

Poller::~Poller() {
  if (pollerfd != -1) {
    close(pollerfd);
    pollerfd = -1;
  }
  delete[] events;
}
// 这里是提取event的，所以要设置Channel的ReadyEvent,所以使用Channel::XXX.
std::vector<Channel *> Poller::poll() {
  std::vector<Channel *> activeEvents;
  int nfds = epoll_wait(pollerfd, events, MAX_NUM, -1);
  error(nfds, "epoll wait error");
  for (int i = 0; i < nfds; ++i) {
    Channel *c = (Channel *)events[i].data.ptr;
    int event = events[i].events;
    if (event & Channel::READ_EVENT)
    {
      c->setRevents(Channel::READ_EVENT);
    }
    if (event & Channel::WRITE_EVENT)
    {
      c->setRevents(Channel::READ_EVENT);
    }
    if (event & Channel::ET)
    {
      c->setRevents(Channel::READ_EVENT);
    }
    activeEvents.push_back(c);
  }
  return activeEvents;
}
// 这里是设置Event的，所以使用EPOLLIN | EPOLLPRI等..
void Poller::updateChannel(Channel *Channel) {
  int fd = Channel->getFd();
  struct epoll_event ev;
  bzero(&ev, sizeof(ev));
  ev.data.ptr = Channel;


///这里进不去有BUG
  if (Channel->getEvent() & Channel::READ_EVENT)
  {
    ev.events |= EPOLLIN | EPOLLPRI;
  }
  if (Channel->getEvent() & Channel::WRITE_EVENT)
  {
    ev.events |= EPOLLOUT;
  }
    if (Channel->getEvent() & Channel::ET)
  {
    ev.events |= EPOLLET;
  }
  if (!Channel->getInEpoll()) {
    error(epoll_ctl(pollerfd, EPOLL_CTL_ADD, fd, &ev),
          "epoll_ctl: add fd error");
    Channel->setEpoll();
  } else {
    error(epoll_ctl(pollerfd, EPOLL_CTL_MOD, fd, &ev),
          "epoll_ctl: modify fd error");
  }
}
#endif

#ifdef OS_MACOS

Poller::Poller() {
  pollerfd = kqueue();
  error(pollerfd, "kqueue create error");
  events = new struct kevent[MAX_NUM];
  memset(events, 0, sizeof(*events) * MAX_NUM);
}

Poller::~Poller() {
  if (pollerfd != -1) {
    close(pollerfd);
  }
}


std::vector<Channel *> Poller::poll() {
  std::vector<Channel *> active_channels;
  struct timespec ts;
  int nfds = 0;
  nfds = kevent(pollerfd, NULL, 0, events, MAX_EVENTS, -1);

  for (int i = 0; i < nfds; ++i) {
    Channel *ch = (Channel *)events[i].udata;
    int event = events[i].filter;
    if (event == EVFILT_READ) {
      ch->setRevents(ch->READ_EVENT | ch->ET);
    }
    if (event == EVFILT_WRITE) {
      ch->setRevents(ch->WRITE_EVENT | ch->ET);
    }
    active_channels.push_back(ch);
  }
  return active_channels;
}

void Poller::updateChannel(Channel *ch) {
  struct kevent ev[2];
  memset(ev, 0, sizeof(*ev) * 2);
  int n = 0;
  int fd = ch.getFd();
  int op = EV_ADD;
  if (ch->getEvent() & ch->ET) {
    op |= EV_CLEAR;
  }
  if (ch->getEvent() & ch->READ_EVENT) {
    EV_SET(&ev[n++], fd, EVFILT_READ, op, 0, 0, ch);
  }
  if (ch->getEvent() & ch->WRITE_EVENT) {
    EV_SET(&ev[n++], fd, EVFILT_WRITE, op, 0, 0, ch);
  }
  int r = kevent(fd_, ev, n, NULL, 0, NULL);
  error(r, "kqueue add event error");
}
#endif