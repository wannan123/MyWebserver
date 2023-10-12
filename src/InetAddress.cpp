#include "include/InetAddress.h"

InetAddress::InetAddress(const char *ip, uint16_t port) {
  bzero(&sock_add, sizeof(sock_add));
  sock_add.sin_family = AF_INET;
  sock_add.sin_addr.s_addr =
      inet_addr(ip); //主机字节序（小端）转网络字节序（大端）
  sock_add.sin_port = htons(
      port); // sockaddr_in是专用socket地址，若用sockaddr则会有一些位移操作很麻烦。
}
InetAddress::InetAddress() { bzero(&sock_add, sizeof(sock_add)); }
InetAddress::~InetAddress() {}

void InetAddress::setInetAddr(sockaddr_in _addr) { sock_add = _addr; }

sockaddr_in InetAddress::getAddr() { return sock_add; }
