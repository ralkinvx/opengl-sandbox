cmake_minimum_required(VERSION 4.3)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(ProjectMandarin VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glfw3 CONFIG REQUIRED)

add_executable(ProjectMandarin
    src/main.cpp
    src/glad.c
)

set(MINGW_RUNTIME_DIR "/usr/x86_64-w64-mingw32/sys-root/mingw/bin")

foreach(dll IN ITEMS
    libstdc++-6.dll
    libgcc_s_seh-1.dll
    libwinpthread-1.dll
)
    if(NOT EXISTS "${MINGW_RUNTIME_DIR}/${dll}")
        message(FATAL_ERROR "Missing MinGW runtime DLL: ${MINGW_RUNTIME_DIR}/${dll}")
    endif()

    add_custom_command(TARGET ProjectMandarin POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                "${MINGW_RUNTIME_DIR}/${dll}"
                "$<TARGET_FILE_DIR:ProjectMandarin>"
        VERBATIM
    )
endforeach()

target_include_directories(ProjectMandarin PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries(ProjectMandarin PRIVATE glfw opengl32 gdi32)

