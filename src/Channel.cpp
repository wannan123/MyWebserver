
#include "Eventloop.h"
#include "Channel.h"
void Channel::handleEvent() {
    callback();
}
void Channel::setCallback(std::function<void()> cb) {
    callback = cb;
}
void Channel::enableReading() {
  events = EPOLLIN | EPOLLET;
  ep->updateChannel(this);
}
int Channel::getFd() { return fd; }

uint32_t Channel::getEvent() { return events; }

uint32_t Channel::getRevents() { return revents; }

bool Channel::getInEpoll() { return inEpoll; }

void Channel::setEpoll() {inEpoll = true;}

void Channel::setRevents(uint32_t revent) {
    revents = revent;
}
void Channel::setEvents(uint32_t event) {
    events = event;
}
Channel::Channel(Eventloop *ep, int fd)
    : ep(ep), fd(fd), events(0), revents(0), inEpoll(false) {}

Channel::~Channel(){
}