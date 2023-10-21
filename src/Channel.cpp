#include "include/Channel.h"
#include "include/Eventloop.h"
#include <unistd.h>
#include "Channel.h"
const int Channel::READ_EVENT = 1;
const int Channel::WRITE_EVENT = 2;
const int Channel::ET = 4;

void Channel::handleEvent() {
  if (revents & (EPOLLIN | EPOLLPRI)) {
    readCallback();
  }
  if (revents & (EPOLLOUT)) {
    writeCallback();
  } // callback();
}
void Channel::setReadCallback(std::function<void()> cb) { readCallback = cb; }
void Channel::EnableReading() {
  events |= READ_EVENT;
  ep->updateChannel(this);
}
void Channel::EnableWriting() {
  events |= WRITE_EVENT;
  ep->updateChannel(this);
}


void Channel::useET() {
  events |= ET;
  ep->updateChannel(this);
}


int Channel::getFd() { return fd; }

uint32_t Channel::getEvent() { return events; }

uint32_t Channel::getRevents() { return revents; }

bool Channel::getInEpoll() { return inEpoll; }

void Channel::setEpoll() { inEpoll = true; }

void Channel::setRevents(int event) { 
  if (event & Channel::READ_EVENT)
  {
    revents |= READ_EVENT;
  }
  if (event & Channel::WRITE_EVENT)
  {
    revents |= WRITE_EVENT;
  }
  if (event & Channel::ET)
  {
    revents |= ET;
  }
}
void Channel::setEvents(uint32_t event) { events = event; }
Channel::Channel(Eventloop *ep, int fd)
    : ep(ep), fd(fd), events(0), revents(0), inEpoll(false) {}

Channel::~Channel() {
  if (fd != -1) {
    close(fd);
    fd = -1;
  }
}
