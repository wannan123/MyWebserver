#pragma once
#include <functional>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
using namespace std;
class ThreadPool
{
    private:
        vector<thread>  threads;
        mutex task_mutex;
        condition_variable cv;
        std::queue<std::function<void()>> tasks;
        bool stop;
        int size;
    public:
        void add(function<void()> func);
        ThreadPool(int size);
        ~ThreadPool();
};

