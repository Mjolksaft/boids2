cmake_minimum_required(VERSION 3.10)
project(Boids VERSION 1.0.0) 

find_package(OpenGL REQUIRED)

add_executable(Boids
    src/main.cpp
    src/glad.c
)

target_include_directories(Boids PRIVATE include)

target_link_libraries(Boids 
    C:/Users/kalla/git/test2/boids/lib/libglfw3dll.a 
    OpenGL::GL
)