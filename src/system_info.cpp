#include "system_info.hpp"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>
#include <lmcons.h>
#include <string>
#include <windows.h>
#include <versionhelpers.h>

void print_graphics_info(GLFWwindow *window) {

    std::cout
        << "==================================================================="
           "=============\n"
           "                    Welcome to Project Mandarin 0.1.0-dev.2!\n"
           "                                 created by Ralkin\n"
           "==================================================================="
           "=============\n"
           "You could see status indicators of certain elements below.\n\n";

    GLint samples = 0;
    GLint sampleBuffers = 0;
    GLint viewport_size[4];
    int max_samples = 0;
    int win_width = 0;
    int win_height = 0;

    glGetIntegerv(GL_MAX_SAMPLES, &max_samples);
    glGetIntegerv(GL_SAMPLES, &samples);
    glGetIntegerv(GL_SAMPLE_BUFFERS, &sampleBuffers);
    glfwGetWindowSize(window, &win_width, &win_height);
    glGetIntegerv(GL_VIEWPORT, viewport_size);

    std::cout << "OpenGL " << glGetString(GL_VERSION) << "\n";
    std::cout << "GLFW " << glfwGetVersionString() << "\n\n";

    std::cout << "GLFW: " << (window ? "ON\n" : "OFF\n");
    std::cout << "GLAD: " << (gladLoadGL() != 0 ? "ON\n" : "OFF\n");
    std::cout << "MSAA: x" << samples << " (max: x" << max_samples << ")\n";
    std::cout << "Initial GLFW Window Dimensions: " << std::to_string(win_width)
              << "x" << std::to_string(win_height) << "\n";
    std::cout << "Initial OpenGL Viewport Dimensions: " << viewport_size[2]
              << "x" << viewport_size[3] << "\n\n";
}

std::wstring get_username() {

    wchar_t buffer[UNLEN + 1];
    DWORD size = UNLEN + 1;

    if (GetUserNameW(buffer, &size)) {
        return buffer;
    }

    return L"Unknown";
}

void print_system_info() {
    const std::wstring username = get_username();

    std::wcout << L"Username: " << username << "\n"; 
    std::cout << "OS: " << (IsWindows10OrGreater() ? "Windows 10 or later" : "Windows 11 or later\n");
};

