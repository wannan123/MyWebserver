#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <vector>
#include "util.h"
#include "Epoll.h"
#include "InetAddress.h"
#include "Socket.h"

#define MAX_EVENTS 10
using namespace std;
void setnonblocking(int clnt_sockfd);
void do_use_fd(int fd);

int main(int argc, const char** argv) {
    
    Socket *serv_sock = new Socket();
    InetAddress *serv_addr = new InetAddress("127.0.0.1", 8880);

    //绑定sockfd文件描述符
    serv_sock->bind(serv_addr);
    serv_sock->listen();  

    //添加epoll
    Epoll *ep = new Epoll();
    serv_sock->setnonblocking();
    ep->addFd(serv_sock->getFd(), EPOLLIN | EPOLLET);
    
    //创建用于监听的描述符
    for(;;){
        //epoll树开始监听，若有准备就绪文件描述符，则写入events内
        vector<struct epoll_event> ready =  ep->poll();
        int nfds = ready.size();
        for (int i = 0; i < nfds; ++i)
        {
            if(ready[i].data.fd == serv_sock->getFd()){//说明服务器fd就绪，有客户端发送请求

                InetAddress *serv_addr = new InetAddress();
                int clnt_sockfd = serv_sock->accept(serv_addr);
                setnonblocking(clnt_sockfd);
                ep->addFd(clnt_sockfd, EPOLLIN | EPOLLET);

            }else if(ready[i].events & EPOLLIN){
                do_use_fd(ready[i].data.fd);
                ep->clear();
            }else{
                cout<<"something else happened"<<endl;
            }
            
        }
        
    }
    serv_sock->~Socket();
    
    return 0;
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