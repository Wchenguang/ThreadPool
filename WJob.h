/*
 * 任务类 抽象类
 * 必须实现run接口
 */

#ifndef _WJOB_H
#define _WJOB_H

class WJob{
public:
    virtual void run() = 0;
};

#endif

