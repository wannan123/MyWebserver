#pragma once
#include <iostream>
#include <string>
class Buffer {
private:
  std::string s;

public:
  void append(const char *_str, int _size);
  void setBuf(const char *_buf);
  ssize_t size();
  const char *c_str();
  void clear();
  void getline();
  Buffer();
  ~Buffer();
};
