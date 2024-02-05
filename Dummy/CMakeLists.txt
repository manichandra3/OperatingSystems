cmake_minimum_required(VERSION 3.27)
project(Dummy)

set(CMAKE_CXX_STANDARD 17)

add_executable(Dummy main.c
        exec1.c
        exec2.c)
