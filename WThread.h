/*
 * 线程的封装
 */

#ifndef _WTHREAD_H
#define _WTHREAD_H

#include <pthread.h>

#include "WJob.h"


//线程类型
typedef pthread_t STD_THREAD_TYPE;
//线程状态
typedef short THRED_STATE_TYPE;
//线程子函数
typedef void *(*TASK_FUNC_TYPE)(void*);


class WThread{
    //绑定的threadPool
    void *threadPool;


    //互斥锁
    pthread_mutex_t myMutex;
    //条件变量
    pthread_cond_t myCond;



private:
    THRED_STATE_TYPE myState;
    WJob *myJob;
    STD_THREAD_TYPE myThread;

    int id;

public:
    //线程状态
    static const THRED_STATE_TYPE T_SLEEP = 0x1;
    static const THRED_STATE_TYPE T_RUNNING = 0x2;
    static const THRED_STATE_TYPE T_EXIT = 0x4;

    WThread(void *pool, int Id);

    void setState(THRED_STATE_TYPE type);

    THRED_STATE_TYPE getState();

    static void *run(WThread *wThread);

    void setJob(WJob *job, bool isSignal);

    WJob *getJob();

    pthread_cond_t *getCond();

    pthread_mutex_t *getMutex();

    void start();

    STD_THREAD_TYPE &getMyThread();

    void *getThreadPool();

    int getId();
};

#endif