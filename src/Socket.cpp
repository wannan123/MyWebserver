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
int Socket::accept(InetAddress *clnt_addr) {
  // socklen_t clnt_addr_len = sizeof(clnt_addr->sock_add);
  // int clnt_sockfd = ::accept(sockfd, (sockaddr *)&clnt_addr->sock_add,
  // &clnt_addr_len); cout<<"new client fd"<<clnt_sockfd<<",IP:
  // "<<inet_ntoa(clnt_addr->sock_add.sin_addr)<<" Port:
  // "<<ntohs(clnt_addr->sock_add.sin_port)<<endl; return clnt_sockfd;
  struct sockaddr_in addr;
  bzero(&addr, sizeof(addr));
  socklen_t addr_len = sizeof(addr);
  int clnt_sockfd = ::accept(sockfd, (sockaddr *)&addr, &addr_len);
  error(clnt_sockfd, "socket accept error");
  clnt_addr->setInetAddr(addr);
  return clnt_sockfd;
}
void Socket::connect(InetAddress *_addr) {
  struct sockaddr_in addr = _addr->getAddr();
  error(::connect(sockfd, (sockaddr *)&addr, sizeof(addr)),
        "socket connect error");
}

Socket::~Socket() {
  if (sockfd != -1) {
    close(sockfd);
    sockfd = -1;
  }
}
