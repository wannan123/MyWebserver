#pragma once
#include "ThreadPool.h"
class Channel;
class Poller;
class Eventloop {
private:
  Poller *ep_;
  bool quit;

public:
  Eventloop();
  ~Eventloop();
  void loop();
  void updateChannel(Channel *);
};
