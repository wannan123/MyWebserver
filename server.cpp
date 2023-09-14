#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <errno.h>
#include <fcntl.h>
#include "Socket.h"
#define MAX_EVENTS 10
using namespace std;
void error(int flag,const char * c){
    if (flag == -1)
    {
        perror(c);
        exit(EXIT_FAILURE);
    }
}
void setnonblocking(int clnt_sockfd){
   //首先去获取文件标记
    int flag = fcntl(clnt_sockfd,F_GETFL);
    if(flag < 0)
    {
        perror("fcntl");
        return;
    }
    //将其设置为非阻塞的方式
    fcntl(clnt_sockfd, F_SETFL, flag|O_NONBLOCK);
}
void do_use_fd(int fd){
    while (true)
    {
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
int main(int argc, const char** argv) {

    Socket *serv_sock = new Socket();
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    error(sockfd,"socket create error");
    //sockaddr_in是专用socket地址，若用sockaddr则会有一些位移操作很麻烦。
    InetAddress *serv_addr = new InetAddress("127.0.0.1", 8888);
    struct sockaddr_in sock_add;
    bzero(&sock_add, sizeof(sock_add));
    sock_add.sin_family = AF_INET;
    sock_add.sin_addr.s_addr = INADDR_ANY;//主机字节序（小端）转网络字节序（大端）
    sock_add.sin_port = htons(8888);
    //绑定sockfd文件描述符

    serv_sock->bind(serv_addr);
    serv_sock->listen();  
    error(bind(sockfd,(sockaddr*)&sock_add,sizeof(sock_add)),"bind error");
    error(listen(sockfd, SOMAXCONN),"listen error");

    //添加epoll
    Epoll *ep = new Epoll();
    int epollfd = epoll_create1(0);
    
    if (epollfd == -1) {
        perror("epoll_create1");
        exit(EXIT_FAILURE);
    }
    struct epoll_event ev, events[MAX_EVENTS];
    bzero(&events, sizeof(events));
    bzero(&ev, sizeof(ev));
    ev.events = EPOLLIN;
    ev.data.fd = sockfd;

    ep->addFd(serv_sock->getFd(), EPOLLIN | EPOLLET);
    error(epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &ev),"epoll_ctl: add severfd error");//将服务器fd添加到epoll树上。

    //创建用于监听的描述符
    for(;;){
        //epoll树开始监听，若有准备就绪文件描述符，则写入events内
        int retfd = epoll_wait(epollfd, events, MAX_EVENTS, -1);
        error(retfd, "epoll_wait error");
        for (int  i = 0; i < retfd; ++i)
        {
            if (events[i].data.fd == sockfd)//说明服务器fd就绪，有客户端发送请求
            {
                struct sockaddr_in clnt_addr;
                socklen_t clnt_addr_len = sizeof(clnt_addr);
                bzero(&clnt_addr, sizeof(clnt_addr));
                int clnt_sockfd = accept(sockfd, (sockaddr*)&clnt_addr, &clnt_addr_len);
                error(clnt_sockfd,"accept error");
                cout<<"new client fd"<<clnt_sockfd<<",IP: "<<inet_ntoa(clnt_addr.sin_addr)<<" Port: "<<ntohs(clnt_addr.sin_port)<<endl;
                setnonblocking(clnt_sockfd);
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = clnt_sockfd;
                error(epoll_ctl(epollfd, EPOLL_CTL_ADD, clnt_sockfd,&ev) == -1,"epoll_ctl: add clnt_sockfd error");
            }else if(events[i].events & EPOLLIN){
                do_use_fd(events[i].data.fd);
                bzero(&events, sizeof(events));
                bzero(&ev, sizeof(ev));
            }else{
                cout<<"something else happened"<<endl;
            }
            
        }
        
    }

    close(sockfd);
    
    return 0;
}
