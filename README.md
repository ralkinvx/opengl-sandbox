# OpenGL Sandbox

Welcome to the repo I use while learning OpenGL! I'm learning and practicing OpenGL here, so things may change.

## Learning Resources
Currently, my main source is <b>"Learn OpenGL - Graphics Programming" by Joey de Vries</b>. You can either <b>buy the book</b> or <b>download the free PDF</b> from [<b>LearnOpenGL.com</b>](https://learnopengl.com/)

I'll add more sources here as I find them.

## Goals

- [x] Hello Window
- [x] Hello Triangle
- [ ] Render a 3D model using cgltf library
- [ ] Shaders
- [ ] Textures

## Build Notes

- C++ Standard: C++23
- OpenGL 4.6
- Toolchain: MinGW-w64 x86_64
- Build System: CMake 4.3
- CMake Generator: Ninja 1.13.2
- Compiler: x86_64-w64-mingw32-g++ (GCC) 16.2.1 (Fedora MinGW 16.2.1-1.fc44)
- Dependencies:
- - Link: glfw3, opengl32, gdi32
- - Statically linked: libgcc, libstdc++
- Editor: VSCodium
- Build Host: Fedora Linux 44 (Workstation Edition)
- Target: Windows x86-64
