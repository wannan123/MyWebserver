#include "Buffer.h"
void Buffer::append(const char *_str, int _size){
    if(_size != 0){
        s.append(_str);
    }
}
void Buffer::setBuf(const char* _buf){
    s.clear();
    s.append(_buf);
}
ssize_t Buffer::size(){
    return s.size();
}
const char *Buffer::c_str()
{
    return s.c_str();
}
void Buffer::clear(){
    s.erase();//不能是clear哦~
}
void Buffer::getline(){
    s.erase();
    std::getline(std::cin, s);
}
Buffer::Buffer() {}

Buffer::~Buffer(){
}
