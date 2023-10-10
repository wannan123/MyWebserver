#include "include/Channel.h"
#include "include/Eventloop.h"
#include <unistd.h>
void Channel::handleEvent() {
  if (revents & (EPOLLIN | EPOLLPRI)) {
    readCallback();
  }
  if (revents & (EPOLLOUT)) {

    writeCallback();
  } // callback();
}
void Channel::setReadCallback(std::function<void()> cb) { readCallback = cb; }
void Channel::enableReading() {
  events = EPOLLIN | EPOLLPRI;
  ep->updateChannel(this);
}

void Channel::useET() {
  events |= EPOLLET;
  ep->updateChannel(this);
}

int Channel::getFd() { return fd; }

uint32_t Channel::getEvent() { return events; }

uint32_t Channel::getRevents() { return revents; }

bool Channel::getInEpoll() { return inEpoll; }

void Channel::setEpoll() { inEpoll = true; }

void Channel::setRevents(uint32_t revent) { revents = revent; }
void Channel::setEvents(uint32_t event) { events = event; }
Channel::Channel(Eventloop *ep, int fd)
    : ep(ep), fd(fd), events(0), revents(0), inEpoll(false) {}

Channel::~Channel() {
  if (fd != -1) {
    close(fd);
    fd = -1;
  }
}