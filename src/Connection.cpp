#include "include/Connection.h"
#include "Marcros.h"
#include "include/Buffer.h"
#include <stdio.h>
#include "Connection.h"

Connection::Connection(Eventloop *_loop, Socket *_sock)
    : loop(_loop), sock(_sock), handleChannel(nullptr) {

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

void Connection::setDeleteConnectionCallback(std::function<void(int)>const & _cb) {
  deleteConnectionCallback = _cb;
}
void Connection::setConnctionCallback(std::function<void(Connection *)> const &_cb) {
  connectCallback_ = _cb;
  handleChannel->setReadCallback([this](){ return connectCallback_(this); });
}
void Connection::Read() {
  if (GetState() == Connection::State::Closed)
  {
    return;
  }
  //ASSERT(state_ == State::Connected, "Connnection is disconnected");
  readBuffer->clear();
  if (sock->IsNonBlocking())
  {
    ReadNonBlocking();
  }else{
    ReadBlocking();
  }
}
void Connection::Write() {
  if (GetState() == Connection::State::Closed)
  {
    return;
  }
  //ASSERT(state_ == State::Connected, "connection state is disconnected!");
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
Socket* Connection::getSocket() { return sock; }

void Connection::Close() { 
  deleteConnectionCallback(sock->getFd()); 
  state_ = State::Closed;
}
void Connection::ReadNonBlocking() {
  int sockfd = sock->getFd();
  char buf[1024];  // 这个buf大小无所谓
  while (true) {   // 使用非阻塞IO，读取客户端buffer，一次读取buf大小数据，直到全部读取完毕
    memset(buf, 0, sizeof(buf));
    ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
    if (bytes_read > 0) {
      readBuffer->append(buf, bytes_read);
    } else if (bytes_read == -1 && errno == EINTR) {  // 程序正常中断、继续读取
      printf("continue reading\n");
      continue;
    } else if (bytes_read == -1 &&
               ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {  // 非阻塞IO，这个条件表示数据全部读取完毕
      break;
    } else if (bytes_read == 0) {  // EOF，客户端断开连接
      printf("read EOF, client fd %d disconnected\n", sockfd);
      state_ = State::Closed;
      break;
    } else {
      printf("Other error on client fd %d\n", sockfd);
      state_ = State::Closed;
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
  ssize_t bytes_write = 0;
  bytes_write = write(sockfd, writeBuffer->c_str(), writeBuffer->size());
  if (bytes_write == 0)
  {
    printf("read EOF, blocking server fd %d disconnected\n", sockfd);
  }
  
  if (bytes_write == -1) {
    printf("Other error on blocking client fd %d\n", sockfd);
    state_ = State::Closed;
  }
}
void Connection::GetlineSendBuffer() { 
  if (GetState() == Connection::State::Closed)
  {
    return;
  }
  writeBuffer->getline(); 
}


