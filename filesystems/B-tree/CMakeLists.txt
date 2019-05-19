cmake_minimum_required(VERSION 3.9)
project(B_tree)

set(CMAKE_CXX_STANDARD 17)

add_executable(B_tree main.cpp BTree.cpp BTree.h)