#include "include/Server.h"
#include "include/Connection.h"
#include "Server.h"
Server::Server(Eventloop *ep) : acceptReactor(ep) {
  acceptor = new Acceptor(acceptReactor);
  std::function<void(Socket *)> cb =
      std::bind(&Server::newConnection, this, std::placeholders::_1);
  acceptor->setNewConnectionCallback(cb);

  int size = std::thread::hardware_concurrency();

  threads = new ThreadPool(size);
  for (int i = 0; i < size; i++) {
    subReactors.push_back(new Eventloop());
    std::function<void()> cb = std::bind(&Eventloop::loop, subReactors[i]);
    threads->add(cb);
  }
}

Server::~Server() {
  delete acceptor;
  delete threads;
}

void Server::newConnection(Socket *serv_sock) {
  if (serv_sock->getFd() != -1) {
    int random = serv_sock->getFd() % subReactors.size(); //调度策略：全随机
    Connection *conn = new Connection(subReactors[random], serv_sock);
    std::function<void(int)> cb =
        std::bind(&Server::deleteConnection, this, std::placeholders::_1);
    conn->setDeleteConnectionCallback(cb);
    conn->setConnctionCallback(connectCallback);
    connections[serv_sock->getFd()] = conn;
  }
}

void Server::deleteConnection(int sockfd) {
  if (sockfd != -1) {
    auto it = connections.find(sockfd);
    if (it != connections.end()) {
      Connection *conn = connections[sockfd];

      connections.erase(sockfd);
      //close(sockfd); //正常
      //delete conn;   // delete conn;         //会Segmant fault
      conn = nullptr;
    }
  }
}
void Server::OnConnect(std::function<void(Connection *)> cb) {
  connectCallback = std::move(cb);
}
