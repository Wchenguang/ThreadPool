#ifndef _WTHREAD_H
#define _WTHREAD_H

#include <thread>
#include <iostream>
#include <string>

#include "WJob.h"

class WThreadPool;


class WThread{
public:

    //线程状态
    enum class THREAD_STATE{
        T_RUNNING,
        T_EXIT,
        T_WAITING
    };

private:
    //线程id
    size_t mId;
    //线程的函数
    WJob *mJob;
    //线程 初始即运行
    std::thread *pmThread;
    //全局锁
    std::mutex mMutex;
    //等待工作的条件变量
    std::condition_variable waitJobCond;
    //所关联的线程池
    WThreadPool &pool;
    //线程状态
    THREAD_STATE mState;

    //线程运行的函数
    void run();

public:


    WThread(WThreadPool &Wpool, size_t id);

    WThread(WThreadPool &Wpool);

    virtual ~WThread();
    //不可 赋值 复制
    WThread(const WThread &) = delete;
    WThread(const WThread &&) = delete;
    WThread &operator = (const WThread&) = delete;

    //设置工作并发布信号
    //无法分辨 job 是栈变量还是堆变量 因此不尝试释放
    //为了提高效率 也不拷贝
    bool setJob(WJob *job);

    //结束
    //是否该结束 由上层来判断
    void terminate();

    //将父线程挂起
    void join();

    bool joinable();

};

#endif