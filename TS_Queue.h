#ifndef _TS_QUEUE_H
#define _TS_QUEUE_H

#include <queue>
#include <exception>
#include <mutex>

#define LOCKALL std::lock_guard<std::mutex> guard(mMutex)

struct EXP_EmptyQueue : std::exception{
    const char *what() const throw(){
        return "Empty Queue\n";
    }
};

template <typename T>
class TS_Queue{
private:
    std::queue<T> mQueue;
    std::mutex mMutex;
public:
    TS_Queue() : mQueue(std::queue<T>()){};

    void push(T &element){
        LOCKALL;
        mQueue.push(element);

    }
    void push(T &&element){
        LOCKALL;
        mQueue.push(element);
        //std::cout<<"push"<<" ok\n";
        //虽然数字杂乱但是push ok 总是成对出现，确实是原子操作
    }

    void pop(T &target){
        LOCKALL;
        target = mQueue.front();
        mQueue.pop();
    }
    std::shared_ptr<T> pop(){
        LOCKALL;
        std::shared_ptr<T> res = std::make_shared(mQueue.front());
        mQueue.pop();
        return res;
    }

    size_t size(){
        LOCKALL;
        return mQueue.size();
    }

    bool empty(){
        LOCKALL;
        return mQueue.empty();
    }
};

#endif