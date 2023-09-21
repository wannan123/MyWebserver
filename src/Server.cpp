#include "Server.h"
Server::Server(Eventloop* ep):ep(ep){
    acceptor = new Acceptor(ep);
    std::function<void(Socket*)> cb = std::bind(&Server::newConnection, this, std::placeholders::_1);
    acceptor->setNewConnectionCallback(cb);
}

Server::~Server(){
}

void Server::handleReadEvent(int fd) {
    while (true){
        char buffer[1024];
        memset(buffer,'\0',sizeof(buffer));
        ssize_t it = read(fd, buffer, sizeof(buffer));
        if (it > 0)
        {
            cout<<"socket:"<<fd<<"said:"<<buffer<<endl;
            write(fd, buffer, sizeof(buffer));
        }else if(it == -1 && errno == EINTR){  //客户端正常中断、继续读取
            continue;
        } else if(it == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))){//非阻塞IO，这个条件表示数据全部读取完毕
            cout<<"该fd上数据读取完毕!"<<endl;
            break;
        }else if (it == 0)
        {
            cout<<"socket:"<<fd<<" closed."<<endl;
            close(fd);
            break;
        }else{
            error(-1, "socket error");
            close(fd);
            break;
        }
    }
}

void Server::newConnection(Socket *serv_sock) {
    InetAddress *serv_addr = new InetAddress();
    Socket *clnt_sockfd =new Socket(serv_sock->accept(serv_addr));
    clnt_sockfd->setnonblocking();
    Channel *channel = new Channel(ep,clnt_sockfd->getFd());
    std::function<void()> c = std::bind(&Server::handleReadEvent,this,clnt_sockfd->getFd());
    channel->setCallback(c);
    channel->enableReading();
    
}
