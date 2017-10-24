#ifndef WTHREADPOOL_H
#define WTHREADPOOL_H

#include <iostream>
#include <queue>
#include <List>
#include <pthread.h>

#include "WThread.h"

//线程类型
typedef pthread_t STD_THREAD_TYPE;

//线程子函数
typedef void *(*TASK_FUNC_TYPE)(void*);

typedef short POOL_STATE_TYPE;




class WThreadPool{
private:



    //初始化线程数
    static const int initThreadNum = 15;
    //线程数
    size_t curThreadNum;
    //用于创建线程分派任务的线程
    STD_THREAD_TYPE dispatchThread;
    //忙碌线程队列
    std::list<WThread *> busyLine;
    //空闲线程队列
    std::list<WThread *> idleLine;
    //任务队列
    std::queue<WJob*> jobLine;
    //线程锁
    pthread_mutex_t myMutex;
    //分派线程等待任务条件变量
    pthread_cond_t DisWaitJobCond;
    //分派线程等待空线程
    pthread_cond_t DisWaitThreadCond;
    //退出所有线程等待
    //pthread_cond_t TerminateThreadCond;
    //待处理任务数量
    size_t jobNum;
    //线程池状态
    POOL_STATE_TYPE poolState;

public:

    static const short P_RUNNING = 0x1;
    static const short P_SLEEP = 0x2;
    static const short P_EXIT = 0x4;

    static void *run(WThreadPool *pool);

    static void *TerminateAll(WThreadPool *pool);

    WThreadPool();

    void pushJob(WJob *job);

    bool hasWaitingThread();

    bool hasWaitingJob();

    void pushToIdleLine(WThread *thread);

    void pushToBusyLine(WThread *thread);

    WJob *popJob();

    WThread *popIdleLine();

    WThread *popBusyLine();


    void removeFromBusyLine(WThread *thread);

    int getjobNum();

    void setThreadNum(size_t num);

    size_t getThreadNum();

    pthread_mutex_t *getMyMutex();

    pthread_cond_t *getDisWaitJobCond();

    pthread_cond_t *getDisWaitThreadCond();

    std::list<WThread*> &getBusyLine();

    std::list<WThread*> &getIdleLine();

    std::queue<WJob*> &getJobLine();

    STD_THREAD_TYPE &getDispatchThread();

    void setState(POOL_STATE_TYPE state);

    //尝试 在所有任务结束后 关闭所有线程
    bool tryTerminate();


    void start();

    POOL_STATE_TYPE getState();
};

#endif