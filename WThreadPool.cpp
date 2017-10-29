#include "WThreadPool.h"

void WThreadPool::run(){
    for(int index = 0; index < curThreadNum; ++index){
        WThread *wThread = new WThread(*this, index);
        pushToIdleLine(wThread);
        pThreadVec.push_back(wThread);
    }

    std::cerr<<"pool init ok\n";


    while (true){
        //等待任务
        std::unique_lock<std::mutex> jlk(mMutex);
        while(jobLine.empty()){
            DisWaitJobCond.wait(jlk);
        }
        jlk.unlock();

        //获取job
        WJob *targetJob;
        jobLine.pop(targetJob);

        //判断退出
        if(poolState == POOL_STATE::P_EXIT && targetJob == EXITJOB)
            break;

        //等待空闲线程
        std::unique_lock<std::mutex> tlk(mMutex);
        while (idleLine.empty()){
            DisWaitThreadCond.wait(tlk);
        }
        tlk.unlock();

        //获取空线程
        WThread *targetThread;
        idleLine.pop_front(targetThread);
        //加入忙碌队列
        pushToBusyLine(targetThread);
        //添加工作
        targetThread->setJob(targetJob);
    }


    //退出所有子线程
    for(int i = 0; i < curThreadNum; ++i){
        pThreadVec[i]->terminate();
        pThreadVec[i]->join();
    }

    std::cerr<<"dispatch thread pool exit\n";

}

WThreadPool::WThreadPool() : curThreadNum(initThreadNum), poolState(POOL_STATE::P_SLEEP), jobNum(0), dispatchThread(nullptr){};

WThreadPool::WThreadPool(size_t threadNum) : WThreadPool() { curThreadNum = threadNum; }

WThreadPool::~WThreadPool(){
    //删除子线程
    for(auto t : pThreadVec){
        if(nullptr != t)
            delete t;
    }
    //删除分发线程
    if(nullptr != dispatchThread)
        delete dispatchThread;
}


void WThreadPool::start(){
    dispatchThread = new std::thread([this](){run();});
}

void WThreadPool::pushJob(WJob *job){
    jobLine.push(job);
    DisWaitJobCond.notify_one();
}

void WThreadPool::getCharToTerminate(){

    getchar();

    poolState = POOL_STATE ::P_EXIT;
    //原子操作
    jobLine.push(EXITJOB);
    DisWaitJobCond.notify_one();

    dispatchThread->join();
}

void WThreadPool::pushToIdleLine(WThread *thread){
    idleLine.push_back(thread);
    DisWaitThreadCond.notify_one();
}

void WThreadPool::pushToBusyLine(WThread *thread){
    busyLine.push_back(thread);
}

void WThreadPool::removeFromBusyLine(WThread *thread){
    busyLine.remove(thread);
}