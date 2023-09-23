#include "Server.h"
#include "Connection.h"
Server::Server(Eventloop* ep):ep(ep){
    acceptor = new Acceptor(ep);
    std::function<void(Socket*)> cb = std::bind(&Server::newConnection, this, std::placeholders::_1);
    acceptor->setNewConnectionCallback(cb);
}

Server::~Server(){
    delete acceptor;
}

void Server::newConnection(Socket *serv_sock) {
    InetAddress *serv_addr = new InetAddress();
    Socket *clnt_sockfd =new Socket(serv_sock->accept(serv_addr));
    clnt_sockfd->setnonblocking();
    Connection *connect = new Connection(ep, clnt_sockfd);
    connections[clnt_sockfd->getFd()] = connect;
    std::function<void(Socket *)> c = std::bind(&Server::deleteConnection,this,clnt_sockfd);
    connect->setCallback(c);
    
}

void Server::deleteConnection(Socket *serv_sock) {
    map<int, Connection *>::iterator it;
    it = connections.find(serv_sock->getFd());
    connections.erase(it);
    delete it->second;
}
