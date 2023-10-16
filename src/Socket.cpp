#include "include/Socket.h"

Socket::Socket() {
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  error(sockfd, "socket create error");
}
Socket::Socket(int fd) {
  sockfd = fd;
  error(sockfd, "socket create error");
}
void Socket::bind(InetAddress *address) {
  error(
      ::bind(sockfd, (sockaddr *)&address->sock_add, sizeof(address->sock_add)),
      "bind error");
}

void Socket::listen() { error(::listen(sockfd, SOMAXCONN), "listen error"); }

int Socket::getFd() { return sockfd; }

void Socket::setnonblocking() {
  fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFL) | O_NONBLOCK);
}
bool Socket::IsNonBlocking() { return (fcntl(sockfd, F_GETFL) & O_NONBLOCK) != 0; }

int Socket::accept(InetAddress *clnt_addr) {
  struct sockaddr_in addr;
  bzero(&addr, sizeof(addr));
  socklen_t addr_len = sizeof(addr);
  int clnt_sockfd = ::accept(sockfd, (sockaddr *)&addr, &addr_len);
  error(clnt_sockfd, "socket accept error");
  clnt_addr->setInetAddr(addr);
  return clnt_sockfd;
}
void Socket::connect(InetAddress *_addr) {
  struct sockaddr_in tmp_addr = _addr->getAddr();
  if (fcntl(sockfd, F_GETFL) & O_NONBLOCK) {
    while (true) {
      int ret = ::connect(sockfd, (sockaddr *)&tmp_addr, sizeof(tmp_addr));
      if (ret == 0) {
        break;
      }
      if (ret == -1 && (errno == EINPROGRESS)) {
        continue;    
        /* 连接非阻塞式sockfd建议的做法：
            The socket is nonblocking and the connection cannot be
          completed immediately.  (UNIX domain sockets failed with
          EAGAIN instead.)  It is possible to select(2) or poll(2)
          for completion by selecting the socket for writing.  After
          select(2) indicates writability, use getsockopt(2) to read
          the SO_ERROR option at level SOL_SOCKET to determine
          whether connect() completed successfully (SO_ERROR is
          zero) or unsuccessfully (SO_ERROR is one of the usual
          error codes listed here, explaining the reason for the
          failure).
          这里为了简单、不断连接直到连接完成，相当于阻塞式
        */
      }
      if (ret == -1) {
        error(true, "socket connect error");
      }
    }
  } else {
    error(::connect(sockfd, (sockaddr *)&tmp_addr, sizeof(tmp_addr)), "socket connect error");
  }
}
void Socket::Connect(const char *ip, uint16_t port) {
  InetAddress *addr = new InetAddress(ip, port);
  connect(addr);
  delete addr;
}

Socket::~Socket() {
  if (sockfd != -1) {
    close(sockfd);
    sockfd = -1;
  }
}
