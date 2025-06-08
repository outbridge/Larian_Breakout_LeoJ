#pragma once

#include <glm/glm.hpp>

struct TransformComponent
{
    glm::vec2 m_Position = { 0.0f, 0.0f };
    glm::vec2 m_Scale = { 1.0f, 1.0f };
    float m_Rotation = 0.0f; // Degrees
};
