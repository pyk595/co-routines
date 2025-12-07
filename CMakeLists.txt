cmake_minimum_required(VERSION 4.0)
project(co-routines)

set(CMAKE_CXX_STANDARD 20)

add_executable(co-routines main.cpp
        thread_pool.cpp
        thread_pool.h
        thread_safe_queue.h)
