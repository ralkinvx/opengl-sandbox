#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <windows.h>


void print_graphics_info(GLFWwindow* window);

std::wstring get_username();

void print_system_info();