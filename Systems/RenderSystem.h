#pragma once

#include "../ECS/System.h"
#include "../ECS/World.h"
#include "../Components/TransformComponent.h"
#include "../Components/RenderableComponent.h"

class RenderSystem : public System
{
public:
    void Init();
    void Update(World& world);

private:
    void SetupGraphicsResources();
    static unsigned int CreateShaderProgram(const char* vsSource, const char* fsSource);

    unsigned int m_ShaderProgram = 0;
    unsigned int m_VAO = 0;
    unsigned int m_VBO = 0;
};

