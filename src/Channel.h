#pragma once 
#include <sys/epoll.h>
#include "Eventloop.h"
#include <iostream>
#include <functional>
// class Epoll;//前向声明
class Channel
{
    private:
        Eventloop *ep;//因为使用了前向声明所以要转化为指针。
        int fd;
        uint32_t events;//表示希望监听这个文件描述符的哪些事件
        uint32_t revents;//表示epoll返回该Channel时文件描述符正在发生的事件
        bool inEpoll;
        //std::function<void()> callback;
        std::function<void()> readCallback;
        std::function<void()> writeCallback;
    public:
        void handleEvent();
        void setReadCallback(std::function<void()> cb);
        void enableReading();
        void useET();
        int getFd();
        uint32_t getEvent();
        uint32_t getRevents();
        bool getInEpoll();
        void setEpoll();
        void setRevents(uint32_t revent);
        void setEvents(uint32_t event);
        Channel(Eventloop *ep, int fd);
        ~Channel();
};


