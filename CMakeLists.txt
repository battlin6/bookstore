cmake_minimum_required(VERSION 3.15)
project(code)

set(CMAKE_CXX_STANDARD 14)

add_executable(code main.cpp User.cpp User.h Book.cpp Book.h)
