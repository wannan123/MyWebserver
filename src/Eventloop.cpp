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
        c = ep_->poll();
        for(auto it = c.begin(); it != c.end(); ++it){
            (*it)->handleEvent();
        }
    }
}

void Eventloop::updateChannel(Channel * c) {
    ep_->updateChannel(c);
}
