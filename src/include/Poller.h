#pragma once
#include "Channel.h"
#include "util.h"
#include "Marcros.h"
#include <strings.h>
#include <sys/epoll.h>
#include <vector>

#define MAX_NUM 10

#ifdef OS_LINUX
  #include <sys/epoll.h>
#endif

#ifdef OS_MACOS
  #include <sys/event.h>  
#endif

class Channel;
class Poller {
private:
  int pollerfd;
  #ifdef OS_LINUX
    struct epoll_event *events{nullptr};
  #endif

  #ifdef OS_MACOS
    struct kevent *events{nullptr};
  #endif

public:
  std::vector<Channel *> poll();
  void updateChannel(Channel *Channel);
  Poller();
  ~Poller();
};
