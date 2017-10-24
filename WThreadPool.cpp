#include "WThreadPool.h"



void *WThreadPool::run(WThreadPool *pool){

    pthread_mutex_t *myMutex = pool->getMyMutex();
    pthread_cond_t *myDisWaitJobCond = pool->getDisWaitJobCond();
    pthread_cond_t *myDisWaitThreadCond = pool->getDisWaitThreadCond();

    pthread_mutex_lock(myMutex);

    for(int i = 0; i < pool->getThreadNum(); ++i){
        WThread *wThread = new WThread(pool, i);
        pool->pushToIdleLine(wThread);
        pthread_join(wThread->getMyThread(), NULL);
        wThread->start();
    }

    pthread_mutex_unlock(myMutex);

    while (1){
        while (!pool->hasWaitingJob())
            pthread_cond_wait(myDisWaitJobCond, myMutex);

        if(pool->getState() == P_EXIT)
            break;

        pthread_mutex_lock(myMutex);

        WJob *job = pool->popJob();

        pthread_mutex_unlock(myMutex);

        while (!pool->hasWaitingThread()){
            pthread_cond_wait(myDisWaitThreadCond, myMutex);
        }
        pthread_mutex_lock(myMutex);
        WThread *thread = pool->popIdleLine();
        pool->pushToBusyLine(thread);
        pthread_mutex_unlock(myMutex);

        thread->setJob(job, true);
    }

    std::cerr<<"dispath end\n";
    return NULL;
}

void *WThreadPool::TerminateAll(WThreadPool *pool) {
    std::list<WThread*> &idleLine = pool->getIdleLine();

    for(auto thr : idleLine){
        thr->setState(WThread::T_EXIT);
        thr->setJob((WJob*)1, true);
    }

    std::cerr<<"all son thread exit\n";
    pthread_exit(NULL);
}

WThreadPool::WThreadPool() : curThreadNum(initThreadNum), jobNum(0), poolState(P_SLEEP){
    //TerminateThreadCond = PTHREAD_COND_INITIALIZER;
    //DisWaitJobCond = PTHREAD_COND_INITIALIZER;
    //DisWaitThreadCond = PTHREAD_COND_INITIALIZER;
    //myMutex = PTHREAD_MUTEX_INITIALIZER;
}

bool WThreadPool::tryTerminate() {
    pthread_mutex_lock(&myMutex);
    std::cerr<<jobLine.size()<<" "<<busyLine.size()<<std::endl;
    if(jobLine.empty() && busyLine.empty()){
        pthread_t terminateThread;

        TASK_FUNC_TYPE func = (TASK_FUNC_TYPE)WThreadPool::TerminateAll;
        pthread_create(&terminateThread, NULL, func, this);
        pthread_join(dispatchThread, NULL);

        std::cerr<<"shit"<<std::endl;

        poolState = P_EXIT;
        pthread_cond_signal(&DisWaitThreadCond);

        pthread_mutex_unlock(&myMutex);
        return true;
    }else{
        pthread_mutex_unlock(&myMutex);
        //pthread_cond_wait(&TerminateThreadCond, &myMutex);
        return false;
    }

}


void WThreadPool::pushJob(WJob *job){
    pthread_mutex_lock(&myMutex);
    jobLine.push(job);
    jobNum = jobLine.size();
    pthread_cond_signal(&DisWaitJobCond);
    pthread_mutex_unlock(&myMutex);
}

bool WThreadPool::hasWaitingThread(){
    return !idleLine.empty();
}

bool WThreadPool::hasWaitingJob(){
    return !(0 == jobNum);
}

void WThreadPool::pushToIdleLine(WThread *thread){
    pthread_mutex_lock(&myMutex);
    idleLine.push_back(thread);
    pthread_cond_signal(&DisWaitThreadCond);
    pthread_mutex_unlock(&myMutex);
}

void WThreadPool::pushToBusyLine(WThread *thread){
    pthread_mutex_lock(&myMutex);
    busyLine.push_back(thread);
    pthread_mutex_unlock(&myMutex);
}

WJob *WThreadPool::popJob(){
    pthread_mutex_lock(&myMutex);
    WJob *job = jobLine.front();
    jobLine.pop();
    jobNum = jobLine.size();
    pthread_mutex_unlock(&myMutex);
    return job;
}

WThread *WThreadPool::popIdleLine(){
    pthread_mutex_lock(&myMutex);
    WThread *thread = idleLine.front();
    idleLine.pop_front();
    pthread_mutex_unlock(&myMutex);
    //pthread_cond_signal(&TerminateThreadCond);
    return thread;
}

void WThreadPool::removeFromBusyLine(WThread *thread){
    pthread_mutex_lock(&myMutex);
    busyLine.remove(thread);
    pthread_mutex_unlock(&myMutex);
}

WThread *WThreadPool::popBusyLine(){
    pthread_mutex_lock(&myMutex);
    WThread *thread = busyLine.front();
    busyLine.pop_front();
    pthread_mutex_unlock(&myMutex);
    return thread;
}

int WThreadPool::getjobNum(){
    return jobNum;
}

void WThreadPool::setThreadNum(size_t num){
    curThreadNum = num;
}

size_t WThreadPool::getThreadNum(){
    return curThreadNum;
}

pthread_mutex_t *WThreadPool::getMyMutex(){
    return &myMutex;
}

pthread_cond_t *WThreadPool::getDisWaitJobCond(){
    return &DisWaitJobCond;
}

pthread_cond_t *WThreadPool::getDisWaitThreadCond(){
    return &DisWaitThreadCond;
}

std::list<WThread*> &WThreadPool::getBusyLine(){
    return busyLine;
}

std::list<WThread*> &WThreadPool::getIdleLine(){
    return idleLine;
}

std::queue<WJob*> &WThreadPool::getJobLine(){
    return jobLine;
}

STD_THREAD_TYPE &WThreadPool::getDispatchThread(){
    return dispatchThread;
}

POOL_STATE_TYPE WThreadPool::getState(){
    return poolState;
}

void WThreadPool::setState(POOL_STATE_TYPE state){
    poolState = state;
}

void WThreadPool::start(){

    poolState = P_RUNNING;

    TASK_FUNC_TYPE func = (TASK_FUNC_TYPE)WThreadPool::run;

    pthread_create(&dispatchThread, NULL, func, this);

}