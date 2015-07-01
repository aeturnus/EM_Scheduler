cmake_minimum_required(VERSION 3.2)
project(EM_Scheduler)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
add_executable(EM_Scheduler ${SOURCE_FILES} Date.cpp Date.h Student.cpp Student.h Shift.cpp Shift.h Day.cpp Day.h)