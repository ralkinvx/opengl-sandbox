#include "shader.hpp"
#include "logger.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <glad/glad.h>


bool shader_program::link_program() {

    logger::log(logger::Severity::INFO, "shader_program", "Shader program value before linking: " + std::to_string(this->uint_program));

    glLinkProgram(this->uint_program);

    int is_successful;
    char linkInfoLog[512];
    glGetProgramiv(this->uint_program, GL_LINK_STATUS, &is_successful);

    if (!is_successful) {
        glGetProgramInfoLog(this->uint_program, 512, NULL, linkInfoLog);
        logger::log(logger::Severity::FATAL, "shader", "Linking shaders to shader program failed.");

        return is_successful;
    }

    return is_successful;
}


void shader::read_shader_data(std::string file) {

    std::ifstream shader_file(file);

    if (!shader_file) {
        logger::log(logger::Severity::ERR, "shader", "Unable to load shader file.");
    }

    std::ostringstream strm;
    strm << shader_file.rdbuf();

    if (strm.str().empty()) {
        logger::log(logger::Severity::WARN, "shader", "Shader file is empty. Expect problems.");
    }

    shader_data_str = strm.str();
    shader_data = shader_data_str.c_str(); 
}

void shader::set_shader_type(shader::Type type) {

    if (type == shader::Type::VERTEX) {
        shader_type = shader::Type::VERTEX;
    } else if (type == shader::Type::FRAGMENT) {
        shader_type = shader::Type::FRAGMENT;
    } else {
        logger::log(logger::Severity::WARN, "shader", "Unspecified or unknown shader type. Use Shader.set_shader_type() first if didn't.");
    }
}

void shader::get_shader_type() {

    std::string type;

    switch (this->shader_type) {
        case Type::FRAGMENT: type = "Shader::Type::FRAGMENT"; break;
        case Type::VERTEX: type = "Shader::Type::VERTEX"; break;
        case Type::UNKNOWN: type = "Shader::Type::UNKNOWN"; break;
        case Type::UNSPECIFIED: type = "Shader::Type::UNSPECIFIED"; break;
    }
    
    logger::log(logger::Severity::INFO, "shader", type);
}

void shader::compile_shader() {

    switch (shader_type) {
        case shader::Type::VERTEX: uint_shader = glCreateShader(GL_VERTEX_SHADER); break;
        case shader::Type::FRAGMENT: uint_shader = glCreateShader(GL_FRAGMENT_SHADER); break;
        case shader::Type::UNKNOWN: logger::log(logger::Severity::ERR, "shader compilation", "Unknown shader type, compilation failed.");
        case shader::Type::UNSPECIFIED: logger::log(logger::Severity::ERR, "shader compilation", "Unspecified shader type. Use Shader.set_type() first.");
    }

    glShaderSource(uint_shader, 1, &this->shader_data, NULL);
    glCompileShader(uint_shader);

    int is_successful;
    char shader_log[512];

    glGetShaderiv(uint_shader, GL_COMPILE_STATUS, &is_successful);

    if (!is_successful) {
        glGetShaderInfoLog(uint_shader, 512, NULL, shader_log);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << shader_log;
    }
}

void shader::attach_shaders(shader_program& program) {

    if (program.uint_program == 0) {
        program.uint_program = glCreateProgram();
        logger::log(logger::Severity::INFO, "shader_program",
                    "created shader program");
    }

    logger::log(logger::Severity::INFO, "shader_program", "object value: " + std::to_string(program.uint_program));

    glAttachShader(program.uint_program, this->uint_shader);
}


