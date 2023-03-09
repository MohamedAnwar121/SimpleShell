cmake_minimum_required(VERSION 3.22)
project(SimpleShell C)

set(CMAKE_C_STANDARD 23)

add_executable(SimpleShell main.c custom-string.h custom-var.h)
