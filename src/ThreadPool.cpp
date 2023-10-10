#include "include/ThreadPool.h"
#include <algorithm>
#include <iostream>
// void ThreadPool::add(function<void()> func){
//     {
//     //在这个{}作用域内对std::mutex加锁，出了作用域会自动解锁，不需要调用unlock()
//         std::unique_lock<std::mutex> lock(task_mutex);
//         if(stop)
//             throw std::runtime_error("ThreadPool already stop, can't add task
//             any more");
//         tasks.emplace(func);
//     }
//     cv.notify_one();    //通知一次条件变量
// }
ThreadPool::ThreadPool(int _size) : size(_size) {
  for (int i = 0; i < _size; i++) {
    threads.emplace_back(thread([this]() {
      // cout<<"create thread and pid is :"<<std::this_thread::get_id()<<endl;
      while (true) {
        function<void()> func;
        {
          unique_lock<mutex> lock(task_mutex);
          cv.wait(lock, [this]() { return stop || !tasks.empty(); });
          if (stop && tasks.empty())
            return;
          func = tasks.front();
          tasks.pop();
        }
        // cout<<std::this_thread::get_id()<<endl;
        func();
      }
    }));
  }
}

ThreadPool::~ThreadPool() {
  {
    std::unique_lock<std::mutex> lock(task_mutex);
    stop = true;
  }
  cv.notify_all();
  // std::cout<<"kakakaka"<<endl;
  std::for_each(threads.begin(), threads.end(),
                [](std::thread &t) { t.join(); });
  // for(std::thread &th : threads){
  //     if(th.joinable())
  //         th.join();
  // }
}
