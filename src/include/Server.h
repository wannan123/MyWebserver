#pragma once
#include "Acceptor.h"
#include "Connection.h"
#include "Eventloop.h"
#include "Socket.h"
#include "ThreadPool.h"
#include "Marcros.h"
#include <functional>
#include <map>
#include <vector>
class Server {
private:
  Eventloop *acceptReactor;
  Acceptor *acceptor;
  std::vector<Eventloop *> subReactors;
  std::map<int, Connection *> connections;
  ThreadPool *threads;
  std::function<void(Connection *)> connectCallback;

public:
  Server(Eventloop *);
  ~Server();
  // void handleReadEvent(int);
  void newConnection(Socket *serv_sock);
  void deleteConnection(int sockfd);
  void OnConnect(std::function<void(Connection *)> cb);
  DISALLOW_COPY_AND_MOVE(Server);
};
