#include <iostream>
#include <functional>
#include <vector>
#include <cmath>
#include <cassert>

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

    #define DIVISIONS 8
    #define CIRCLE (2 * M_PI)
    #define ANGLE (CIRCLE / DIVISIONS)
    std::vector<float> vertices = {.0f, .0f, .0f};
    std::vector<unsigned int> indices;

    for (size_t division_n = 0; division_n < DIVISIONS; division_n++)
    {
        float angle_n = division_n * ANGLE;

        vertices.push_back(std::cos(angle_n));  // x
        vertices.push_back(std::sin(angle_n));  // y
        vertices.push_back(.0f);                // z

        indices.push_back(0);                                       // center
        indices.push_back(division_n + 1);                          // current xyz
        indices.push_back(((division_n + 1) % DIVISIONS) + 1);      // next (expected) xyz
    }

    assert((DIVISIONS + 1) * 3 == vertices.size());
    assert((DIVISIONS) * 3 == indices.size());

    std::function<void(int)> verifyShaderCompilationStatus = [](unsigned int id)
    {
        int status;
        glGetShaderiv(id, GL_COMPILE_STATUS, &status);
        if (!status)
        {
            char infoLog[512] = {0};
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

    unsigned int vertexArrayObjectId;
    glGenVertexArrays(1, &vertexArrayObjectId);
    glBindVertexArray(vertexArrayObjectId);

    unsigned int arrayBufferObjectId;
    glGenBuffers(1, &arrayBufferObjectId);
    glBindBuffer(GL_ARRAY_BUFFER, arrayBufferObjectId);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    unsigned int elementArrayBufferObjectId;
    glGenBuffers(1, &elementArrayBufferObjectId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementArrayBufferObjectId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

#if 0
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(.2f, .3f, .3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return EXIT_SUCCESS;
}