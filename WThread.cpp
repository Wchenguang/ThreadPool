#include "WThread.h"
#include "WThreadPool.h"

void WThread::run(){

    std::string str = std::string("thread init") + std::to_string(mId) + std::string("\n");
    std::cerr<<str;

    while (true){
        //当job是NULL时 阻塞等待信号
        //std::lock_guard<std::mutex> g(mMutex);
        std::unique_lock<std::mutex> lk(mMutex);
        while (nullptr == mJob){
            waitJobCond.wait(lk);
        }
        lk.unlock();

        //如果为exit 即退出 线程结束
        if(mState == THREAD_STATE::T_EXIT){
            break;
        }

        mJob->run();

        //将条件置回 以及 放入空闲链中 是原子操作
        mJob = nullptr;

        pool.removeFromBusyLine(this);
        pool.pushToIdleLine(this);

    }

    str = std::string("thread ") + std::to_string(mId) + std::string(" in pool exit\n");
    std::cerr<<str;
}

WThread::WThread(WThreadPool &Wpool, size_t id) :pool(Wpool), mJob(nullptr), mState(THREAD_STATE::T_RUNNING), mId(id) {
    pmThread = new std::thread([this](){run();});
}

WThread::WThread(WThreadPool &Wpool) : WThread(Wpool, -1) {
};

WThread::~WThread(){}


//设置工作并发布信号
//无法分辨 job 是栈变量还是堆变量 因此不尝试释放
//为了提高效率 也不拷贝
bool WThread::setJob(WJob *job){
    //防止job工作时改变job 导致遗失job
    if(nullptr != mJob){
        return false;
    }
    mJob = job;
    waitJobCond.notify_one();
    return true;
}

//结束
//是否该结束 由上层来判断
void WThread::terminate(){
    mState = THREAD_STATE ::T_EXIT;
    while (!setJob((WJob*)1));
}

void WThread::join(){
    if(pmThread->joinable())
        pmThread->join();
}

bool WThread::joinable(){
    return pmThread->joinable();
}