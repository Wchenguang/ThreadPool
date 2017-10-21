/*
 * 线程的封装
 */

#include <pthread.h>

#include "WJob.h"

//线程类型
typedef pthread_t STD_THREAD_TYPE;
//线程状态
typedef short THRED_STATE_TYPE;
//线程子函数
typedef void *(*TASK_FUNC_TYPE)(void*);


class WThread{
    //线程状态
    static const THRED_STATE_TYPE T_SLEEP = 0x1;
    static const THRED_STATE_TYPE T_RUNNING = 0x2;
    static const THRED_STATE_TYPE T_EXIT = 0x4;

    //互斥锁
    pthread_mutex_t myMutex;
    //条件变量
    pthread_cond_t myCond;

private:
    THRED_STATE_TYPE myState;
    WJob *myJob;
    STD_THREAD_TYPE myThread;


public:
    void setState(THRED_STATE_TYPE type){
        myState = type;
    }

    WThread() : myState(T_SLEEP), myJob(EMPTY_JOB)  {
        myMutex = PTHREAD_MUTEX_INITIALIZER;
        myCond = PTHREAD_COND_INITIALIZER;
    };

    static void *run(WThread *wThread){
        wThread->setState(T_RUNNING);
        pthread_mutex_t *mutex = wThread->getMutex();
        pthread_cond_t *cond = wThread->getCond();

        while (wThread->getJob() == EMPTY_JOB){

            pthread_cond_wait(cond, mutex);
        }


        void *res =  wThread->getJob()->run();


        return res;

    }

    void setJob(WJob *job){
        pthread_mutex_lock(&myMutex);

        myJob = job;

        pthread_mutex_unlock(&myMutex);

        pthread_cond_signal(&myCond);
    }

    WJob *getJob(){
        return myJob;
    }

    pthread_cond_t *getCond(){
        return &myCond;
    }

    pthread_mutex_t *getMutex(){
        return &myMutex;
    }

    void start(){

        TASK_FUNC_TYPE taskFunc = (TASK_FUNC_TYPE)WThread::run;

        pthread_create(&myThread, NULL, taskFunc, this);
    }

    STD_THREAD_TYPE &getMyThread(){
        return myThread;
    }
};