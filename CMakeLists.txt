cmake_minimum_required(VERSION 3.15)
project(DbREST C)

set(CMAKE_C_STANDARD 99)

add_executable(DbREST main.c parser.c cache.c typedefs.h)