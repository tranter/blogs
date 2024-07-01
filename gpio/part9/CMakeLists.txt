cmake_minimum_required(VERSION 3.5)

project(example LANGUAGES C)

add_executable(${PROJECT_NAME} example.c)
target_link_libraries(${PROJECT_NAME} gpiod)
