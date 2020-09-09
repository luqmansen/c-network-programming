cmake_minimum_required(VERSION 3.17)
project(daytime C)

set(CMAKE_C_STANDARD 11)

add_executable(server.c)
add_executable(client.o client.c)