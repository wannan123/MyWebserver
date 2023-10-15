#include "include/Connection.h"
#include "Marcros.h"
#include "include/Buffer.h"
#include <stdio.h>
#include "Connection.h"

Connection::Connection(Eventloop *_loop, Socket *_sock)
    : loop(_loop), sock(_sock), handleChannel(nullptr) {

  state_ = Connected;
  readBuffer = new Buffer();
    if (loop != nullptr) {
    handleChannel = new Channel(loop, sock->getFd());
    handleChannel->enableReading();
    handleChannel->useET();
  }
  readBuffer = new Buffer();
  writeBuffer = new Buffer();
  state_ = State::Connected;
}

Connection::~Connection() {
  if (loop != nullptr) {
    delete handleChannel;
  }
  delete sock;
  delete readBuffer;
  delete writeBuffer;
}

void Connection::setDeleteConnectionCallback(std::function<void(int)> _cb) {
  deleteConnectionCallback = _cb;
}
void Connection::setConnctionCallback(std::function<void(Connection *)> _cb) {
  connectCallback_ = _cb;
}
void Connection::Read() {
  ASSERT(state_ == State::Connected, "Connnection is disconnected");
  readBuffer->clear();
  if (sock->IsNonBlocking())
  {
    ReadNonBlocking();
  }else{
    ReadBlocking();
  }
}
void Connection::Write() {
  ASSERT(state_ == State::Connected, "connection state is disconnected!");
  if (sock->IsNonBlocking()) {
    WriteNonBlocking();
  } else {
    WriteBlocking();
  }
  writeBuffer->clear();
}
void Connection::SetSendBuffer(const char *_buf) {
  writeBuffer->setBuf(_buf);
}
const char *Connection::ReadBuffer() { return readBuffer->c_str(); }
const char *Connection::WriteBuffer() { return writeBuffer->c_str(); }
Buffer *Connection::GetReadBuffer() { return readBuffer; }
Buffer *Connection::GetWriteBuffer() { return writeBuffer; }

Connection::State Connection::GetState() { return state_; }

void Connection::Close() { deleteConnectionCallback(sock->getFd()); }

void Connection::ReadNonBlocking() {
  int fd = sock->getFd();
  char buffer[1024];
  while (true) {
    memset(buffer, '\0', sizeof(buffer));
    ssize_t bytes_read = read(fd, buffer, sizeof(buffer));
    if (bytes_read > 0) {
      readBuffer->append(buffer, bytes_read);
    } else if (bytes_read == -1 &&
               errno == EINTR) { //  客户端正常中断、继续读取
      printf("continue reading");
      continue;
    } else if (bytes_read == -1 &&
               ((errno == EAGAIN) ||
                (errno ==
                 EWOULDBLOCK))) { //  非阻塞IO，这个条件表示数据全部读取完毕
      printf("finish reading once\n");
      printf("message from client fd %d: %s\n", fd, readBuffer->c_str());
      //  error(write(fd, readBuffer->c_str(), readBuffer->size()) == -1,
      //  "socket write error");
      // readBuffer->clear();
      break;
    } else if (bytes_read == 0) { // EOF，客户端断开连接
      printf("EOF, client fd %d disconnected\n", fd);
      // close(sockfd),关闭socket会自动将文件描述符从epoll树上移除
      // deleteConnectionCallback(fd);
      state_ = Closed;
      break;
    } else {
      printf("Connection reset by peer\n");
      // 这里关闭和删除逻辑在sever或client实现。deleteConnectionCallback(fd); // 会有bug，注释后单线程无bug
      state_ = Closed;  // 这里可以改成Faild;
      break;
    }
  }
}
void Connection::ReadBlocking() {
  int sockfd = sock->getFd();
  unsigned int rcv_size = 0;
  socklen_t len = sizeof(rcv_size);
  getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &rcv_size, &len);
  char buf[rcv_size];
  ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
  if (bytes_read > 0) {
    readBuffer->append(buf, bytes_read);
  } else if (bytes_read == 0) {
    printf("read EOF, blocking client fd %d disconnected\n", sockfd);
    state_ = State::Closed;
  } else if (bytes_read == -1) {
    printf("Other error on blocking client fd %d\n", sockfd);
    state_ = State::Closed;
  }
}
void Connection::WriteNonBlocking() {
  int sockfd = sock->getFd();
  char buf[writeBuffer->size()];
  memcpy(buf, writeBuffer->c_str(), writeBuffer->size());
  int data_size = writeBuffer->size();
  int data_left = data_size;
  while (data_left > 0) {
    ssize_t bytes_write = write(sockfd, buf + data_size - data_left, data_left);
    if (bytes_write == -1 && errno == EINTR) {
      printf("continue writing\n");
      continue;
    }
    if (bytes_write == -1 && errno == EAGAIN) {
      break;
    }
    if (bytes_write == -1) {
      printf("Other error on client fd %d\n", sockfd);
      state_ = State::Closed;
      break;
    }
    data_left -= bytes_write;
  }
}
void Connection::WriteBlocking() {
  int sockfd = sock->getFd();
  ssize_t bytes_write = write(sockfd, writeBuffer->c_str(), writeBuffer->size());
  if (bytes_write == -1) {
    printf("Other error on blocking client fd %d\n", sockfd);
    state_ = State::Closed;
  }
}
void Connection::GetlineSendBuffer() { writeBuffer->getline(); }


