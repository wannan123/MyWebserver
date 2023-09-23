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
    
    return 0;
}

