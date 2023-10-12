#pragma once
#include <arpa/inet.h>
#include <iostream>
#include <strings.h>
class InetAddress {
public:
    struct sockaddr_in sock_add;

public:
    InetAddress(const char *ip, uint16_t port);
    InetAddress();
    ~InetAddress();
    void setInetAddr(sockaddr_in _addr);
    sockaddr_in getAddr();
};
