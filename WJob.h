/*
 * 任务类 抽象类
 * 必须实现run接口
 */


//typedef void *(*TASK_FUNC_TYPE)(void*);
//typedef void* TASK_ARGS_TYPE;

//const TASK_FUNC_TYPE EMPTY_FUNC = 0;
//const TASK_ARGS_TYPE EMPTY_ARGS = 0;

//空
#define EMPTY_JOB NULL

class WJob{
public:
    virtual void *run() = 0;
};

/*template <typename argType>
class WJobWithArgs : public WJob{
private:
    //完成任务的函数
    TASK_FUNC_TYPE  myFunc;
    //任务函数参数
    argType         myArgs;
public:
    WJobWithArgs(TASK_FUNC_TYPE func = EMPTY_FUNC, argType args = EMPTY_ARGS){
        setFunc(func);
        setArgs(args);
    }
    virtual ~WJobWithArgs() {
        if(myArgs != EMPTY_ARGS)
            delete myArgs;
    }

    void *run(){
        return myFunc(myArgs);
    }

    void setFunc(TASK_FUNC_TYPE func){
        myFunc = func;
    }
    void setArgs(argType args){
        myArgs = new argType();
        *myArgs = *args;
    }
};*/