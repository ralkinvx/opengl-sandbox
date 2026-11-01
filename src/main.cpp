#include <cstddef>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "logger.hpp"
#include "system_info.hpp"
#include "shader.hpp"

extern "C" {
    __declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;               // Request NVIDIA Optimus drivers for Windows (not guaranteed, if not working, use Windows Graphics Settings to run with dGPU)
    __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;         // Request AMD drivers for Windows (not guaranteed, if not working, use Windows Graphics Settings to run with dGPU)

    // For Linux systems with Wine, use "__NV_PRIME_RENDER_OFFLOAD=1 __GLX_VENDOR_LIBRARY_NAME=nvidia wine <executable-name>.exe"
}

void resize_function(GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    }

void input_handler(GLFWwindow* window) {
    
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

int main(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Project Mandarin 0.1.0-dev.3", NULL, NULL);

    if (window == NULL) {logger::log(logger::Severity::FATAL, "glfw", "Failed to create GLFW Window."); glfwTerminate(); return -1;}

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        logger::log(logger::Severity::FATAL, "glad", "Failed to initialize GLAD.");
        return -1;
    }

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, resize_function);

    print_graphics_info(window);
    print_system_info();

    logger::self_test();

    logger::log(logger::Severity::INFO, "logger", "*****************************************************************************************");
    logger::log(logger::Severity::INFO, "logger", "************************************ LOGGING STARTED ************************************");
    logger::log(logger::Severity::INFO, "logger", "*****************************************************************************************");

    // ************************************************************************
    // ************************** SHADER SETUP START **************************
    // ************************************************************************

    shader sh_vertexShader;
    sh_vertexShader.read_shader_data("../assets/shaders/basic.vert");
    sh_vertexShader.set_shader_type(shader::Type::VERTEX);
    sh_vertexShader.compile_shader();

    shader sh_fragmentShader;
    sh_fragmentShader.read_shader_data("../assets/shaders/basic.frag");
    sh_fragmentShader.set_shader_type(shader::Type::FRAGMENT);
    sh_fragmentShader.compile_shader();
    

    shader_program program;
    sh_fragmentShader.attach_shaders(program);
    sh_vertexShader.attach_shaders(program);

    program.link_program();
    
    // ************************************************************************
    // *************************** SHADER SETUP END ***************************
    // ************************************************************************


    // ************************************************************************
    // **************************** RENDERING START ***************************
    // ************************************************************************


    GLuint vbos[2];                                                          // Vertex Buffer Array (VBO) for storing vertex data.
    GLuint vaos[2];

    // unsigned int EBO;             
    // glGenBuffers(1, &EBO);                                           // Vertex Attributes Array (VAO) for storing vertex attribute data.
    glGenBuffers(2, vbos);                                           // Generate the buffer.

    glGenVertexArrays(2, vaos);

        float vertices[] = {                                                        // Vertex position attribute float array.
        0.5, 0.5, 0.0,                                            // This is an interleaved array (there'll be separate arrays for different attributes such as color)
        0.5, -0.5, 0.0,
        -0.5, -0.5, 0.0,
        -0.5, -0.5, 0.0,                                          
        -0.5, 0.5, 0.0,                                          
        0.5, 0.5, 0.0,                                          

    };

    for (int i = 0; i <= 1; i++) {
        glBindVertexArray(vaos[i]);
        glBindBuffer(GL_ARRAY_BUFFER, vbos[i]);                          // Specify the buffer type (GL_ARRAY_BUFFER, as we'll store an array) and the buffer object.
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);             // Store the vertices[] position data in the buffer we've created, and specify the usage type.

        switch (i) {
            case 0:
            glVertexAttribPointer(
            0,                    // Index number of data we want to configure. To configure location we use 0. ("layout (location = 0) in vec3 aPos;" is the first argument in vertex shader)
            3,                     // Size of the vertex data. Since it's a vec3, (x,y,z), we use 3.
            GL_FLOAT,              // Type of the data.
            GL_FALSE,        // Normalize integers to float value boolean. Since we're already using floats, there's nothing to normalize.
            3 * sizeof(float),   // !! IMPORTANT: For non-interleaved data (all attributes within the same array), the number of values to skip, starting from the first attribute. You CANNOT assign RGB color values while passing position data, so you have to skip these values.
            (void*)0            // Where position data begins in the buffer. I have no idea why we cast this as void pointer... I don't want to know.
            );
            break;
            case 1:
            glVertexAttribPointer(
            0,                   
            3,                
            GL_FLOAT,              
            GL_FALSE,      
            3 * sizeof(float),   
            (void*)(9 * sizeof(float))        
            );
            break;
        }

        glEnableVertexAttribArray(0);           // Enable the attribute array we've created (this is disabled by default so don't forget to put this line or rendering probably won't work)

    }

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);                   

    // unsigned int indices[] = {
    //     1, 2, 3,
    //     3, 1, 0
    // };

    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    


    glDeleteShader(sh_vertexShader.uint_shader);                                       // When linked properly to the shader program, shaders should be deleted.
    glDeleteShader(sh_fragmentShader.uint_shader);



    // ************************************************************************
    // **************************** RENDERING END *****************************
    // ************************************************************************


    while (!glfwWindowShouldClose(window)) {

        glEnable(GL_MULTISAMPLE);
        glEnable(GL_DEBUG_OUTPUT);

        input_handler(window);

        //glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program.uint_program);                                       // Every rendering call under this line will use the specified shader program.
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glBindVertexArray(vaos[0]);       
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(vaos[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    logger::log(logger::Severity::INFO, "logger", "*****************************************************************************************");
    logger::log(logger::Severity::INFO, "logger", "************************************ LOGGING FINISHED ***********************************");
    logger::log(logger::Severity::INFO, "logger", "*****************************************************************************************");
    
    return 0;
}
