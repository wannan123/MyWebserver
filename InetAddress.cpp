#include "InetAddress.h"


InetAddress::InetAddress(const char* ip, uint16_t port){
    bzero(&sock_add, sizeof(sock_add));
    sock_add.sin_family = AF_INET;
    sock_add.sin_addr.s_addr = inet_addr(ip);
    sock_add.sin_port = htons(port);

}
InetAddress::InetAddress(){
    bzero(&sock_add, sizeof(sock_add));
}
InetAddress::~InetAddress()
{
}