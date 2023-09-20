#include "Epoll.h"
#include "Eventloop.h"
#include "Channel.h"
#include <vector>
Eventloop::Eventloop():ep_(nullptr), quit(false){
    ep_ = new Epoll();
}

Eventloop::~Eventloop(){
    delete ep_;
}

void Eventloop::loop() {
    while(!quit){
    std::vector<Channel*> c;
    /*此处相当于把所有活跃的channel给poll出来，其中包括一开始
    处理NewConnection的channel,也有处理读的channel。抽象出来相当于
    NewConnection为父节点，然后一旦有新Socket来连接就创建子节点HandleReadEvent*/
        c = ep_->poll();
        for(auto it = c.begin(); it != c.end(); ++it){
            (*it)->handleEvent();
        }
    }
}

void Eventloop::updateChannel(Channel * c) {
    ep_->updateChannel(c);
}
