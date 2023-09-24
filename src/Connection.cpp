#include "Connection.h"
#include "Buffer.h"

Connection::Connection(Eventloop *_loop, Socket *_sock) : loop(_loop), sock(_sock), handleChannel(nullptr){
    handleChannel = new Channel(loop, sock->getFd());
    std::function<void()> cb = std::bind(&Connection::echo, this, sock->getFd());
    handleChannel->setCallback(cb);
    handleChannel->enableReading();
    readBuffer = new Buffer();
    
}

Connection::~Connection(){
    cout<<"connection delete"<<endl;
    delete sock;
    delete handleChannel;
}

void Connection::echo(int fd) {    
    while (true){
        char buffer[1024];
        memset(buffer,'\0',sizeof(buffer));
        ssize_t bytes_read = read(fd, buffer, sizeof(buffer));
        if(bytes_read > 0){
            readBuffer->append(buffer, bytes_read);
        } else if(bytes_read == -1 && errno == EINTR){  //客户端正常中断、继续读取
            printf("continue reading");
            continue;
        } else if(bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))){//非阻塞IO，这个条件表示数据全部读取完毕
            printf("finish reading once\n");
            printf("message from client fd %d: %s\n", fd, readBuffer->c_str());
            error(write(fd, readBuffer->c_str(), readBuffer->size()) == -1, "socket write error");
            readBuffer->clear();
            break;
        } else if(bytes_read == 0){  //EOF，客户端断开连接
            printf("EOF, client fd %d disconnected\n", fd);
            // close(sockfd);   //关闭socket会自动将文件描述符从epoll树上移除
            deleteConnections(sock);
            break;
        }
    }
    
}

void Connection::setCallback(std::function<void(Socket *)> cb) {
    deleteConnections = cb;
}
