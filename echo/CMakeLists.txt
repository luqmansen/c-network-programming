cmake_minimum_required(VERSION 3.17)
project(daytime C)

set(CMAKE_C_STANDARD 11)

add_executable(server.o echoserver.c)
add_executable(client.o echoclient.c)