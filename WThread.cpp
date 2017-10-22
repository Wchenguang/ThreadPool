#include "WThread.h"

#include "WThreadPool.h"

WThread::WThread(void *pool, int Id) : myState(T_SLEEP), myJob(EMPTY_JOB), id(Id) {
    myMutex = PTHREAD_MUTEX_INITIALIZER;
    myCond = PTHREAD_COND_INITIALIZER;
    threadPool = pool;
};

void WThread::setState(THRED_STATE_TYPE type){
    myState = type;
}

void *WThread::run(WThread *wThread){

    wThread->setState(T_RUNNING);
    pthread_mutex_t *mutex = wThread->getMutex();
    pthread_cond_t *cond = wThread->getCond();

    while(1){
        while (wThread->getJob() == EMPTY_JOB){

            pthread_cond_wait(cond, mutex);
        }

        if(wThread->getState() == WThread::T_EXIT)
            break;

        if(wThread->getState() == WThread::T_EXIT)
            break;

        wThread->getJob()->run();

        //?
        //销毁job
        delete wThread->getJob();
        wThread->setJob(EMPTY_JOB, false);

        //从busyline删除 加入idleline
        WThreadPool *pool = (WThreadPool*)wThread->getThreadPool();

        pool->removeFromBusyLine(wThread);
        pool->pushToIdleLine(wThread);


    }

    return NULL;

}

void *WThread::getThreadPool(){
    return threadPool;
}



void WThread::setJob(WJob *job, bool isSignal){
    myJob = job;
    if(isSignal)
        pthread_cond_signal(&myCond);
}

WJob *WThread::getJob(){
    return myJob;
}

pthread_cond_t *WThread::getCond(){
    return &myCond;
}

pthread_mutex_t *WThread::getMutex(){
    return &myMutex;
}

void WThread::start(){

    TASK_FUNC_TYPE taskFunc = (TASK_FUNC_TYPE)WThread::run;

    pthread_create(&myThread, NULL, taskFunc, this);
}

THRED_STATE_TYPE WThread::getState(){
    return myState;
}


STD_THREAD_TYPE &WThread::getMyThread(){
    return myThread;
}

int WThread::getId(){
    return id;
}