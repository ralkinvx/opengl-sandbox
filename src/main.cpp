#include <cstddef>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "system_info.hpp"

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

    GLFWwindow* window = glfwCreateWindow(800, 600, "Project Mandarin 0.1.0-dev.2", NULL, NULL);

    if (window == NULL) {std::cout << "Failed to create GLFW Window"; glfwTerminate(); return -1;}

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, resize_function);

    print_graphics_info(window);
    print_system_info();

    // ************************************************************************
    // ************************** SHADER SETUP START **************************
    // ************************************************************************


    const char* vertexShaderSource = "#version 460\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main() {\n"
    "gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int vertSuccess;
    char vertInfoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertSuccess);

    if (!vertSuccess) {
        glGetShaderInfoLog(vertexShader, 512, NULL, vertInfoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << vertInfoLog << std::endl;
    }

    const char* fragmentShaderSource = "#version 460\n"
    "out vec4 FragColor;\n"
    "void main() {\n"
    "FragColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);\n"
    "}\0";

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    int fragSuccess;
    char fragInfoLog[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragSuccess);

    if (!fragSuccess) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, fragInfoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << fragInfoLog << std::endl;
    }

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int linkSuccess;
    char linkInfoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkSuccess);

    if (!linkSuccess) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, linkInfoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << linkInfoLog << std::endl;
    }


    // ************************************************************************
    // *************************** SHADER SETUP END ***************************
    // ************************************************************************


    // ************************************************************************
    // **************************** RENDERING START ***************************
    // ************************************************************************


    unsigned int VBO;                                                           // Vertex Buffer Array (VBO) for storing vertex data.
    unsigned int VAO; 
    unsigned int EBO;             
    glGenBuffers(1, &EBO);                                           // Vertex Attributes Array (VAO) for storing vertex attribute data.
    glGenBuffers(1, &VBO);                                           // Generate the buffer.

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);                          
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);                   
    glBindBuffer(GL_ARRAY_BUFFER, VBO);                          // Specify the buffer type (GL_ARRAY_BUFFER, as we'll store an array) and the buffer object.


    float vertices[] = {                                                        // Vertex position attribute float array.
        0.5, 0.5, 0.0,                                            // This is an interleaved array (there'll be separate arrays for different attributes such as color)
        0.5, -0.5, 0.0,
        -0.5, -0.5, 0.0,
        -0.5, 0.5, 0.0
    };

    unsigned int indices[] = {
        1, 2, 3,
        3, 1, 0
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);             // Store the vertices[] position data in the buffer we've created, and specify the usage type.
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(
        0,                    // Index number of data we want to configure. To configure location we use 0. ("layout (location = 0) in vec3 aPos;" is the first argument in vertex shader)
        3,                     // Size of the vertex data. Since it's a vec3, (x,y,z), we use 3.
        GL_FLOAT,              // Type of the data.
        GL_FALSE,        // Normalize integers to float value boolean. Since we're already using floats, there's nothing to normalize.
        3 * sizeof(float),   // !! IMPORTANT: For non-interleaved data (all attributes within the same array), the number of values to skip, starting from the first attribute. You CANNOT assign RGB color values while passing position data, so you have to skip these values.
        (void*)0            // Where position data begins in the buffer. I have no idea why we cast this as void pointer... I don't want to know.
    );

    glEnableVertexAttribArray(0);           // Enable the attribute array we've created (this is disabled by default so don't forget to put this line or rendering probably won't work)

    glDeleteShader(vertexShader);                                       // When linked properly to the shader program, shaders should be deleted.
    glDeleteShader(fragmentShader);



    // ************************************************************************
    // **************************** RENDERING END *****************************
    // ************************************************************************


    while (!glfwWindowShouldClose(window)) {

        glEnable(GL_MULTISAMPLE);

        input_handler(window);

        //glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);                                       // Every rendering call under this line will use the specified shader program.
        glBindVertexArray(VAO);       
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
