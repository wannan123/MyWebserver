#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <vector>
#include "src/util.h"
#include "src/Eventloop.h"
#include "src/Server.h"

#define MAX_EVENTS 10
using namespace std;

int main(int argc, const char** argv) {
    //前面两句相当于做了创建socket,绑定，监听，创建Eventloop,创建acceptor
    Eventloop *loop = new Eventloop();
    Server *server = new Server(loop);
    loop->loop();//不断处理事件
    
    // Socket *serv_sock = new Socket();
    // InetAddress *serv_addr = new InetAddress("127.0.0.1", 8880);

    // //绑定sockfd文件描述符
    // serv_sock->bind(serv_addr);
    // serv_sock->listen();  

    // //添加epoll
    // Epoll *ep = new Epoll();
    // serv_sock->setnonblocking();

    // Channel *channel = new Channel(ep,serv_sock->getFd());
    // channel->enableReading();
    
    // //创建用于监听的描述符
    // for(;;){
    //     //epoll树开始监听，若有准备就绪文件描述符，则写入events内
    //     vector<Channel*> ready =  ep->poll();
    //     int nfds = ready.size();
    //     for (int i = 0; i < nfds; ++i)
    //     {
    //         if(ready[i]->getFd() == serv_sock->getFd()){//说明服务器fd就绪，有客户端发送请求

    //             InetAddress *serv_addr = new InetAddress();
    //             Socket *clnt_sockfd =new Socket(serv_sock->accept(serv_addr));
    //             clnt_sockfd->setnonblocking();
    //             Channel *channel = new Channel(ep,clnt_sockfd->getFd());
    //             channel->enableReading();

    //         }else if(ready[i]->getRevents() & EPOLLIN){
    //             do_use_fd(ready[i]->getFd());
    //             ep->clear();
    //         }else{
    //             cout<<"something else happened"<<endl;
    //         }
            
    //     }
        
    // }
    // serv_sock->~Socket();
    
    return 0;
}

