#include <iostream>
#include <functional>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <shaders/basic_vertex.generated.hpp>
#include <shaders/basic_fragment.generated.hpp>

static void onFrameBufferSizeCallback(
    GLFWwindow *window,
    int width,
    int height)
{
    glViewport(0, 0, width, height);
}

static void onKeyCallback(
    GLFWwindow *window,
    int key,
    int scancode,
    int action,
    int mods)
{
    std::cout << "Key callback: " << key << " action: " << action << std::endl;
}

int main(int, char **)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwSetFramebufferSizeCallback(window, onFrameBufferSizeCallback);
    glfwSetKeyCallback(window, onKeyCallback);

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f
    };

    std::function<void(int)> verifyShaderCompilationStatus = [](unsigned int id)
    {
        int status;
        glGetShaderiv(id, GL_COMPILE_STATUS, &status);
        if (!status)
        {
            char infoLog[512] = { 0 };
            glGetShaderInfoLog(id, sizeof(infoLog), nullptr, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED" << infoLog << std::endl;
        }
    };

    unsigned int vertexShaderId;
    vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShaderId, 1, &basic_vertex, nullptr);
    glCompileShader(vertexShaderId);
    verifyShaderCompilationStatus(vertexShaderId);

    unsigned int fragmentShaderId;
    fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderId, 1, &basic_fragment, nullptr);
    glCompileShader(fragmentShaderId);
    verifyShaderCompilationStatus(fragmentShaderId);

    unsigned int shaderProgramId;
    shaderProgramId = glCreateProgram();
    glAttachShader(shaderProgramId, vertexShaderId);
    glAttachShader(shaderProgramId, fragmentShaderId);
    glLinkProgram(shaderProgramId);
    glUseProgram(shaderProgramId);

    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);

    unsigned int vertexBufferObjectId;
    glGenBuffers(1, &vertexBufferObjectId);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjectId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    unsigned int vertexArrayObjectId;
    glGenVertexArrays(1, &vertexArrayObjectId);
    glBindVertexArray(vertexArrayObjectId);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(.2f, .3f, .3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return EXIT_SUCCESS;
}