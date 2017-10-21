#include <iostream>

//线程子函数
typedef void *(*TASK_FUNC_TYPE)(void*);

class WThreadPool{
private:
    static const int initThreadNum = 25;

    size_t curThreadNum;
    pthread_t dispatchThread;

public:

    static void *run(WThreadPool *pool){

    }

    WThreadPool() : curThreadNum(initThreadNum){

    }

    void setThreadNum(size_t num){
        curThreadNum = num;
    }

    void start(){

        TASK_FUNC_TYPE func = (TASK_FUNC_TYPE)WThreadPool::run;

        pthread_create(&dispatchThread, NULL, func, this);


    }
};