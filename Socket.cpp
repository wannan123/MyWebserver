#include "Socket.h"


Socket::Socket(){
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    ep = new Epoll();
    error(sockfd,"socket create error");
}
inline void Socket::bind(InetAddress *address) {
    error(::bind(sockfd,(sockaddr*)&address,sizeof(address)),"bind error");
}

inline void Socket::listen() {
    error(::listen(sockfd, SOMAXCONN),"listen error");
}
void Socket::setnonblocking(int clnt_sockfd){
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
void Socket::do_use_fd(int fd) {
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
int Socket::getFd() { return sockfd; }


int Socket::accept(InetAddress *clnt_addr) {
    socklen_t clnt_addr_len = sizeof(clnt_addr);
    int clnt_sockfd = ::accept(sockfd, (sockaddr *)&clnt_addr, &clnt_addr_len);
    return clnt_sockfd;
}

Socket::~Socket(){
}
