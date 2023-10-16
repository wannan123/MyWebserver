#include <iostream>
#include <string>

#include "ThreadPool.h"

void Print(int a, double b, const char *c, std::string const &d) { std::cout << a << b << c << d << std::endl; }

void Test() { std::cout << "hellp" << std::endl; }

int main(int argc, char const *argv[]) {
  ThreadPool *poll = new ThreadPool(10);
  std::function<void()> func = std::bind(Print, 1, 3.14, "hello", std::string("world"));
  poll->add(func);
  func = Test;
  poll->add(func);
  delete poll;
  return 0;
}
