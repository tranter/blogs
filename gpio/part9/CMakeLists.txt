cmake_minimum_required(VERSION 3.5)

project(gpiod LANGUAGES C)

add_executable(example example.c)
target_link_libraries(example gpiod)
