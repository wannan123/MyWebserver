#pragma once
#include <arpa/inet.h>
#include <iostream>
#include <strings.h>
class InetAddress
{
    public:
        struct sockaddr_in sock_add;
    public:
        InetAddress(std::string ip, int port);
        ~InetAddress();
};


