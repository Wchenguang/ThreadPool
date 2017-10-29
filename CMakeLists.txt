cmake_minimum_required(VERSION 3.8)
project(ThreadPool)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp
        WJob.h
        WThread.h
        WThread.cpp
        WThreadPool.h
        WThreadPool.cpp
        TS_List.h
        TS_Queue.h
        )
add_executable(ThreadPool ${SOURCE_FILES})