#include "Acceptor.h"
#include "Server.h"
Acceptor::Acceptor(Eventloop *ep):loop(ep){
    sock = new Socket();
    addr = new InetAddress("127.0.0.1", 8890);

    //绑定sockfd文件描述符
    sock->bind(addr);
    sock->listen();  

    //添加epoll
    sock->setnonblocking();
    acceptChannel = new Channel(loop,sock->getFd());
    std::function<void()> cb = std::bind(&Acceptor::acceptConnection, this);
    acceptChannel->setCallback(cb);
    acceptChannel->enableReading();
    delete addr;
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
