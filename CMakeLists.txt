cmake_minimum_required(VERSION 3.15)
project(bookstore)

set(CMAKE_CXX_STANDARD 14)

add_executable(bookstore main.cpp User.cpp User.h Book.cpp Book.h)