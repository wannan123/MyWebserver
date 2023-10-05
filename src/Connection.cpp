#include "Connection.h"
#include "Buffer.h"

Connection::Connection(Eventloop *_loop, Socket *_sock) : loop(_loop), sock(_sock), handleChannel(nullptr){
    handleChannel = new Channel(loop, sock->getFd());
    std::function<void()> cb = std::bind(&Connection::echo, this, sock->getFd());
    handleChannel->setReadCallback(cb);
    handleChannel->setUseThreadPool(true);
    handleChannel->enableReading();
    handleChannel->useET();
    readBuffer = new Buffer();
    
}

Connection::~Connection(){
    cout<<"connection delete"<<endl;
    delete sock;
    delete handleChannel;
    delete readBuffer;
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
            //error(write(fd, readBuffer->c_str(), readBuffer->size()) == -1, "socket write error");
            send(fd);
            readBuffer->clear();
            break;
        } else if(bytes_read == 0){  //EOF，客户端断开连接
            printf("EOF, client fd %d disconnected\n", fd);
            // close(sockfd);   //关闭socket会自动将文件描述符从epoll树上移除
            deleteConnectionCallback(fd);
            break;
        } else {
            printf("Connection reset by peer\n");
            deleteConnectionCallback(fd);          //会有bug，注释后单线程无bug
            break;
        }
    }
    
}


void Connection::setDeleteConnectionCallback(std::function<void(int)> _cb){
    deleteConnectionCallback = _cb;
}
void Connection::send(int sockfd){
    char buf[readBuffer->size()];
    strcpy(buf, readBuffer->c_str());
    int  data_size = readBuffer->size(); 
    int data_left = data_size; 
    while (data_left > 0) 
    { 
        ssize_t bytes_write = write(sockfd, buf + data_size - data_left, data_left); 
        if (bytes_write == -1 && errno == EAGAIN) { 
            break;
        }
        data_left -= bytes_write; 
    }
}