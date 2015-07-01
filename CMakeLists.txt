cmake_minimum_required(VERSION 3.2)
project(EM_Scheduler)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -static")

set(SOURCE_FILES main.cpp)
#include_directories(C:\libs\include)
#link_directories(C:\libs\lib)
#add_library(libxls libxls.a )

include_directories(C:/libs/xlslib/include)
link_directories(C:/libs/xlslib/lib)
add_executable(EM_Scheduler ${SOURCE_FILES} Date.cpp Date.h Student.cpp Student.h Shift.cpp Shift.h Day.cpp Day.h)
target_link_libraries(EM_Scheduler C:/libs/xlslib/lib/libxls.a )