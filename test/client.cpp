#include "util.h"
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
using namespace std;

int main(int argc, const char **argv) {
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in serv_addr;
  bzero(&serv_addr, sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  serv_addr.sin_port = htons(8890);
  error(connect(sockfd, (sockaddr *)&serv_addr, sizeof(serv_addr)),
        "connect error");

  while (true) {
    char buffer[1024];              //定义缓冲区
    bzero(&buffer, sizeof(buffer)); //清空缓冲区
    cin >> buffer;                  //从键盘输入要传到服务器的数据
    ssize_t write_bytes = write(
        sockfd, buffer,
        sizeof(buffer)); //发送缓冲区中的数据到服务器socket，返回已发送数据大小
    if (write_bytes == -1) { // write返回-1，表示发生错误
      cout << "socket already disconnected, can't write any more!" << endl;
      break;
    }
    memset(buffer, '\0', sizeof(buffer)); //清空缓冲区
    ssize_t read_bytes =
        read(sockfd, buffer,
             sizeof(buffer)); //从服务器socket读到缓冲区，返回已读数据大小
    if (read_bytes > 0) {
      cout << "message from server:" << buffer << endl;
    } else if (read_bytes ==
               0) { // read返回0，表示EOF，通常是服务器断开链接，等会儿进行测试
      cout << "server socket disconnected!" << endl;
      break;
    } else if (read_bytes ==
               -1) { // read返回-1，表示发生错误，按照上文方法进行错误处理
      close(sockfd);
      error(-1, "socket read error");
    }
  }
  close(sockfd);
  return 0;
}
