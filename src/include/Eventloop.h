#pragma once
#include "ThreadPool.h"
class Channel;
class Epoll;
class Eventloop {
private:
  Epoll *ep_;
  bool quit;

public:
  Eventloop();
  ~Eventloop();
  void loop();
  void updateChannel(Channel *);
};
