cmake_minimum_required(VERSION 3.27)
project(OperatingSystems)

set(CMAKE_CXX_STANDARD 14)

include_directories(Assignment3)

add_executable(OperatingSystems
        Assignment1/Q5/convert.c
        Assignment1/Q5/count.c
        Assignment1/Q5/Q5.c
        Assignment1/Q6/change.c
        Assignment1/Q6/count.c
        Assignment1/Q6/Q6.c
        Assignment1/CmdArgs.cpp
        Assignment1/NonAlNum.c
        Assignment1/Q4.c
        Assignment1/SysCalls.c
        Assignment1/ToUppercase.c
        Assignment3/cpu-scheduler.c
        Assignment3/globals.h
        Assignment3/header.h
        Assignment4/DiningPhilosophers.c
        Assignment5/DetectDeadlock.c
        Dummy/exec1.c
        Dummy/exec2.c
        Dummy/jignesh.c
        Dummy/main.c
        Misc/Bankers.c
        Misc/DetectDeadlock.c
        Misc/DiningPhilosophers.c
        Misc/Producer-Consumer.c
        test1/2201074.c
        test2/testing/primeChk.c
        test2/Child.c
        test2/Parent.c
        test3/2201074_tes3.c
        test3/test.c)
