cmake_minimum_required(VERSION 3.9)
project(Signaturer)

set(CMAKE_CXX_STANDARD 11)

add_executable(Signaturer bserver.cpp bserver.h bclient.cpp bclient.h bserver_attack.cpp bserver_attack.h)