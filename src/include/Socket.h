#pragma once
#include "Poller.h"
#include "InetAddress.h"
#include "util.h"
#include <arpa/inet.h>
#include <cstring>
#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>

class Socket {
private:
  int sockfd;

public:
  Poller *ep;
  void bind(InetAddress *address);
  void listen();
  void setnonblocking();
  int accept(InetAddress *fd);
  void connect(InetAddress *);
  void Connect(const char *ip, uint16_t port);
  bool IsNonBlocking();
  int getFd();
  Socket();
  Socket(int fd);
  ~Socket();
};
