cmake_minimum_required(VERSION 2.8...3.13)
project(Glad)
message(PROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}")

add_library(glad include/glad/glad.h src/glad.c)
target_include_directories(glad PUBLIC include/)