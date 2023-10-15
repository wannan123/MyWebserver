#pragma once
#include "Buffer.h"
#include "Eventloop.h"
#include "Socket.h"
#include "Marcros.h"
#include <functional>
class Eventloop;
class Socket;
class Channel;
class Connection {  
public:
  enum State {
    Invalid = 1,
    Handshaking,
    Connected,
    Closed,
    Failed,
  };
  Connection(Eventloop *_loop, Socket *_sock);
  ~Connection();
  void setDeleteConnectionCallback(std::function<void(int)> _cb);
  void setConnctionCallback(std::function<void(Connection *)> _cb);

  void Read();
  void Write();

  void SetSendBuffer(const char *);
  const char * ReadBuffer();
  const char * WriteBuffer();
  Buffer *GetReadBuffer();
  Buffer *GetWriteBuffer();

  
  void ReadNonBlocking();
  void ReadBlocking();
  void WriteNonBlocking();
  void WriteBlocking();
  void GetlineSendBuffer();

  State GetState();
  void Close();

private:
  Eventloop *loop;
  Socket *sock;
  Channel *handleChannel;
  std::function<void(int)> deleteConnectionCallback;
  std::function<void(Connection *)> connectCallback_;
  Buffer *readBuffer;
  Buffer *writeBuffer;
  DISALLOW_COPY_AND_MOVE(Connection);  // 禁止拷贝和移动构造；
  State state_{State::Invalid};
};
