#include <iostream>
#include <map>
#include "Connection.h"
#include "Eventloop.h"
#include "Server.h"
#include "Socket.h"

int main() {
  std::map<int, Connection *> clients;

  Eventloop *loop = new Eventloop();
  Server *server = new Server(loop);

  server->SetNewConnect(
      [&](Connection *conn) {
      int clnt_fd = conn->getSocket()->getFd();
      std::cout << "New connection fd: " << clnt_fd << std::endl;
      clients[clnt_fd] = conn;
  });

  server->OnConnect(
    [&](Connection *conn){
      conn->Read();
      conn->SetSendBuffer(conn->ReadBuffer());
      std::cout << "Message from client " << conn->ReadBuffer() << std::endl;
      for(auto &each : clients){
        Connection *client = each.second;
        int clnt_fd = client->getSocket()->getFd();
        std::cout << "this fd is : " << clnt_fd << std::endl;
        client->SetSendBuffer(conn->ReadBuffer());
        std::cout << "Message from client " << conn->getSocket()->getFd() << ": " << conn->ReadBuffer() << std::endl;
        client->Write();
      }
    }
  );
    
  loop->loop();
  return 0;
}


