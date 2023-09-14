#include "InetAddress.h"

InetAddress::InetAddress(std::string ip, int port){
    bzero(&sock_add, sizeof(sock_add));    
    sock_add.sin_family = AF_INET;
    sock_add.sin_addr.s_addr = INADDR_ANY;//主机字节序（小端）转网络字节序（大端）
    sock_add.sin_port = htons(8888);
}
InetAddress::~InetAddress(){
}