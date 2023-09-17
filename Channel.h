#pragma once 
#include <sys/epoll.h>
class Epoll;//前向声明
class Channel
{
    private:
        Epoll *ep;//因为使用了前向声明所以要转化为指针。
        int fd;
        uint32_t events;//表示希望监听这个文件描述符的哪些事件
        uint32_t revents;//表示epoll返回该Channel时文件描述符正在发生的事件
        bool inEpoll;
    public:
        void enableReading();
        int getFd();
        uint32_t getEvent();
        uint32_t getRevents();
        bool getInEpoll();
        void setEpoll();
        void setRevents(uint32_t revent);
        void setEvents(uint32_t event);
        Channel(Epoll *ep, int fd);
        ~Channel();
};


