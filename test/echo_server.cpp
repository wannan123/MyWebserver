#include "Server.h"
#include "Eventloop.h"
#include "errno.h"
#include "util.h"
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <vector>
#define MAX_EVENTS 10

int main(int argc, const char **argv) {
  //  前面两句相当于做了创建socket,绑定，监听，创建Eventloop,创建acceptor
  Eventloop *acceptLoop = new Eventloop();

  Server *server = new Server(acceptLoop);
  server->OnConnect([](Connection *conn){
    conn->Read();
    if(conn->GetState() == Connection::State::Closed){
      conn->Close();
      return;
    }
    std::cout << "Message from client " << conn->getSocket()->getFd() << ": " << conn->ReadBuffer() << std::endl;
    conn->SetSendBuffer(conn->ReadBuffer());
    conn->Write();  
  });

  acceptLoop->loop(); //  不断处理事件
  delete server;
  delete acceptLoop;
  return 0;
}
