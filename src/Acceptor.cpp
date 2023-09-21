#include "Acceptor.h"
#include "Server.h"
Acceptor::Acceptor(Eventloop *ep){
    sock = new Socket();
    addr = new InetAddress("127.0.0.1", 8870);

    //绑定sockfd文件描述符
    sock->bind(addr);
    sock->listen();  

    //添加epoll
    sock->setnonblocking();
    acceptChannel = new Channel(ep,sock->getFd());
    std::function<void()> cb = std::bind(&Acceptor::acceptConnection, this);
    acceptChannel->setCallback(cb);
    acceptChannel->enableReading();
}

Acceptor::~Acceptor(){
    delete sock;
    delete addr;
    delete acceptChannel;
}

void Acceptor::acceptConnection() {
    newConnections(sock);
}

void Acceptor::setNewConnectionCallback(std::function<void(Socket *)> cb){
    newConnections = cb;
}
