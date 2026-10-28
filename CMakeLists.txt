cmake_minimum_required(VERSION 4.3)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(ProjectMandarin VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glfw3 CONFIG REQUIRED)

add_executable(ProjectMandarin
    src/main.cpp
    src/glad.c
    src/system_info.cpp
)

target_link_options(ProjectMandarin PRIVATE
    -static
    -static-libgcc
    -static-libstdc++
)

target_include_directories(ProjectMandarin PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries(ProjectMandarin PRIVATE glfw opengl32 gdi32)

