#pragma once
#include <functional>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
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
        // void add(function<void()> func);
        template<class F, class... Args>
        auto add(F&& f, Args&&... args) 
        -> std::future<typename std::result_of<F(Args...)>::type>;
        ThreadPool(int size);
        ~ThreadPool();
};
template<class F, class... Args>
auto ThreadPool::add(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type> {
    using return_type = typename std::result_of<F(Args...)>::type;  //返回值类型

    auto task = std::make_shared< std::packaged_task<return_type()> >(  //使用智能指针
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)  //完美转发参数
        );  
        
    std::future<return_type> res = task->get_future();  // 使用期约
    {   //队列锁作用域
        std::unique_lock<std::mutex> lock(task_mutex);   //加锁

        if(stop)
            throw std::runtime_error("enqueue on stopped ThreadPool");

        tasks.emplace([task](){ (*task)(); });  //将任务添加到任务队列
    }
    cv.notify_one();    //通知一次条件变量
    return res;     //返回一个期约
}
