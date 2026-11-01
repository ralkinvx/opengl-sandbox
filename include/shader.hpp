#pragma once

#include <string>
#include <glad/glad.h>


class shader_program {
  
    public:
    bool link_program();
    unsigned int uint_program = 0;
};

class shader {

    public:
    enum class Type {
        FRAGMENT,
        VERTEX,
        UNSPECIFIED,
        UNKNOWN
    };
    
    std::string name;
    enum Type shader_type = shader::Type::UNSPECIFIED;
    const GLchar* shader_data;

    void read_shader_data(std::string file);
    void set_shader_type(shader::Type);
    void get_shader_type();
    void compile_shader();
    void attach_shaders(shader_program& program);    

    unsigned int uint_shader;

    private:
    std::string shader_data_str;
};

