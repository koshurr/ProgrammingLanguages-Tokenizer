cmake_minimum_required(VERSION 3.8)
project(Tokenizer)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c Tokenizer.c Tokenizer.h parseTree.h)
add_executable(Tokenizer ${SOURCE_FILES})