#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
using namespace std;
void error(int flag,const char * c){
    if (flag == -1)
    {
        perror(c);
        exit(EXIT_FAILURE);
    }
}
int main(int argc, const char** argv) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    error(sockfd,"socket create error");
    //sockaddr_in是专用socket地址，若用sockaddr则会有一些位移操作很麻烦。

    struct sockaddr_in sock_add;
    bzero(&sock_add, sizeof(sock_add));
    sock_add.sin_family = AF_INET;
    sock_add.sin_addr.s_addr = INADDR_ANY;//主机字节序（小端）转网络字节序（大端）
    sock_add.sin_port = htons(8888);
    //绑定sockfd文件描述符
    error(bind(sockfd,(sockaddr*)&sock_add,sizeof(sock_add)),"bind error");
    error(listen(sockfd, SOMAXCONN),"listen error");

    //创建用于监听的描述符
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_len = sizeof(clnt_addr);
    bzero(&clnt_addr, sizeof(clnt_addr));
    int clnt_sockfd = accept(sockfd, (sockaddr*)&clnt_addr, &clnt_addr_len);
    error(clnt_sockfd,"accept error");
    cout<<"new client fd"<<clnt_sockfd<<",IP: "<<inet_ntoa(clnt_addr.sin_addr)<<" Port: "<<ntohs(clnt_addr.sin_port)<<endl;

    while (true)
    {
        char buffer[1024];
        memset(buffer,'\0',sizeof(buffer));
        ssize_t it = read(clnt_sockfd, buffer, sizeof(buffer));
        if (it > 0)
        {
            cout<<"socket:"<<clnt_sockfd<<"said:"<<buffer<<endl;
            write(clnt_sockfd, buffer, sizeof(buffer));
        }else if (it == 0)
        {
            cout<<"socket:"<<clnt_sockfd<<" closed."<<endl;
            close(clnt_sockfd);
            break;
        }else{
            error(-1, "socket error");
            close(clnt_sockfd);
            break;
        }
    }
    close(sockfd);
    close(clnt_sockfd);
    
    return 0;
}
