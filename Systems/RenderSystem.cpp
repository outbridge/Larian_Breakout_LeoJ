#include "RenderSystem.h"
#include "../Components/TransformComponent.h"
#include "../Components/RenderableComponent.h"
#include "../Core/Types.h"
#include <glad/glad.h>
#include <iostream>

const char* vertexShaderSrc = R"(
#version 330 core
layout (location = 0) in vec2 aPos;

uniform vec2 uPosition;  // already in NDC
uniform vec2 uScale;     // already in NDC

void main()
{
    vec2 finalPos = aPos * uScale + uPosition;
    gl_Position = vec4(finalPos, 0.0, 1.0);
}
)";

const char* fragmentShaderSrc = R"(
#version 330 core
out vec4 FragColor;
uniform vec3 uColor;

void main()
{
    FragColor = vec4(uColor, 1.0);
}
)";

// --- Shader helpers ---

unsigned int CompileShader(unsigned int type, const char* source)
{
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "[Shader Compile Error]:\n" << infoLog << std::endl;
    }

    return shader;
}

unsigned int RenderSystem::CreateShaderProgram(const char* vsSource, const char* fsSource)
{
    unsigned int vertexShader = CompileShader(GL_VERTEX_SHADER, vsSource);
    unsigned int fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fsSource);

    unsigned int program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cerr << "[Shader Link Error]:\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

// --- RenderSystem methods ---

void RenderSystem::Init()
{
    SetupGraphicsResources();
    m_ShaderProgram = CreateShaderProgram(vertexShaderSrc, fragmentShaderSrc);
}

void RenderSystem::SetupGraphicsResources()
{
    float vertices[] = {
        // quad: CCW from bottom-left
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.5f,  0.5f,
        -0.5f,  0.5f
    };

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void RenderSystem::Update(World& world)
{
    glUseProgram(m_ShaderProgram);
    glBindVertexArray(m_VAO);

    glm::vec2 screenSize = { 800.0f, 600.0f }; // Match your window size

    for (Entity entity : GetEntities())
    {
        auto& transform = world.GetComponent<TransformComponent>(entity);
        auto& renderable = world.GetComponent<RenderableComponent>(entity);

        // Convert position and scale from pixel space to NDC space (-1 to 1)
        glm::vec2 posNDC = (transform.m_Position / screenSize) * 2.0f - 1.0f;
        glm::vec2 scaleNDC = transform.m_Scale / screenSize * 2.0f;

        GLint posLoc = glGetUniformLocation(m_ShaderProgram, "uPosition");
        GLint scaleLoc = glGetUniformLocation(m_ShaderProgram, "uScale");
        GLint colorLoc = glGetUniformLocation(m_ShaderProgram, "uColor");

        glUniform2f(posLoc, posNDC.x, posNDC.y);
        glUniform2f(scaleLoc, scaleNDC.x, scaleNDC.y);
        glUniform3f(colorLoc, renderable.m_Color.r, renderable.m_Color.g, renderable.m_Color.b);

        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    }

    glBindVertexArray(0);
    glUseProgram(0);
}