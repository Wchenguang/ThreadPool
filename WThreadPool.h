#ifndef _WTHREADPOOL_H
#define _WTHREADPOOL_H

#include <thread>
#include <queue>
#include <vector>

#include "WThread.h"
#include "TS_List.h"
#include "TS_Queue.h"

class WThreadPool{
public:
    enum class POOL_STATE{
        P_RUNNING,
        P_EXIT,
        P_SLEEP
    };
private:

    friend class WThread;

    //退出的标志任务
    WJob *EXITJOB = (WJob*)1;

    //初始化线程数
    static const int initThreadNum = 15;
    //线程数
    size_t curThreadNum;

    //用于 静态保存 池内线程的指针
    std::vector<WThread*> pThreadVec;
    //忙碌线程队列
    TS_List<WThread *> busyLine;
    //空闲线程队列
    TS_List<WThread *> idleLine;
    //任务队列
    TS_Queue<WJob*> jobLine;
    //线程锁
    std::mutex mMutex;
    //分派线程等待任务条件变量
    std::condition_variable DisWaitJobCond;
    //分派线程等待空线程
    std::condition_variable DisWaitThreadCond;
    //初始化运行线程池 变量
    std::condition_variable initPoolCond;
    //待处理任务数量
    size_t jobNum;
    //线程池状态
    POOL_STATE poolState;

    //用于创建线程分派任务的线程
    std::thread *dispatchThread;

    //分发线程的函数
    void run();

    //将线程压入空闲队列
    void pushToIdleLine(WThread *thread);
    //压入忙碌队列
    void pushToBusyLine(WThread *thread);
    //找到并从忙碌队列中移除
    void removeFromBusyLine(WThread *thread);

public:
    WThreadPool();

    WThreadPool(size_t threadNum);

    virtual ~WThreadPool();

    WThreadPool(const WThreadPool &) = delete;
    WThreadPool(WThreadPool &&) = delete;
    WThreadPool &operator = (const WThreadPool&) = delete;

    //开始接受任务
    void start();

    //添加job
    void pushJob(WJob *job);

    //用于父线程 输入任意字符结束
    void getCharToTerminate();



};

#endif