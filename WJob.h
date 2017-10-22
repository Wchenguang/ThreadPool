/*
 * 任务类 抽象类
 * 必须实现run接口
 */

#ifndef _WJOB_H
#define _WJOB_H

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

#endif

