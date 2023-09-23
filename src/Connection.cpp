#include "Connection.h"

Connection::Connection(Eventloop *_loop, Socket *_sock) : loop(_loop), sock(_sock), handleChannel(nullptr){
    handleChannel = new Channel(loop, sock->getFd());
    std::function<void()> cb = std::bind(&Connection::echo, this, sock->getFd());
    handleChannel->setCallback(cb);
    handleChannel->enableReading();
}

Connection::~Connection(){
    cout<<"connection delete"<<endl;
    delete sock;
    delete handleChannel;
}

void Connection::echo(int fd) {    while (true){
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
            deleteConnections(sock);
            break;
        }else{
            error(-1, "socket error");
            close(fd);
            break;
        }
    }
}

void Connection::setCallback(std::function<void(Socket *)> cb) {
    deleteConnections = cb;
}
