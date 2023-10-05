#include "Server.h"
#include "Connection.h"
Server::Server(Eventloop* ep):ep(ep){
    acceptor = new Acceptor(ep);
    std::function<void(Socket*)> cb = std::bind(&Server::newConnection, this, std::placeholders::_1);
    acceptor->setNewConnectionCallback(cb);
}

Server::~Server(){
    delete acceptor;
    delete ep;
}

void Server::newConnection(Socket *serv_sock) {
    // InetAddress *serv_addr = new InetAddress();
    // Socket *clnt_sockfd =new Socket(serv_sock->accept(serv_addr));
    // clnt_sockfd->setnonblocking();
    // Connection *connect = new Connection(ep, clnt_sockfd);
    // std::function<void(int)> cb = std::bind(&Server::deleteConnection, this, std::placeholders::_1);
    // connect->setDeleteConnectionCallback(cb);
    // connections[clnt_sockfd->getFd()] = connect;

    if(serv_sock->getFd() != -1){
        Connection *conn = new Connection(ep, serv_sock);
        std::function<void(int)> cb = std::bind(&Server::deleteConnection, this, std::placeholders::_1);
        conn->setDeleteConnectionCallback(cb);
        connections[serv_sock->getFd()] = conn;
    }
    

}

void Server::deleteConnection(int sockfd) {

    if(sockfd != -1){
        auto it = connections.find(sockfd);
        if(it != connections.end()){
            Connection *conn = connections[sockfd];
            //close(sockfd);       //正常
            connections.erase(sockfd);
            
            delete conn;         //会Segmant fault
        }
    }
}

