#pragma once
#include <glm/glm.hpp>

struct PhysicsComponent
{
    glm::vec2 m_Velocity{ 0.0f };
    glm::vec2 m_Acceleration{ 0.0f };
    float m_Friction = 5.0f;         // Higher = stronger deceleration
    bool m_IsKinematic = false;

    PhysicsComponent() = default;

    PhysicsComponent(glm::vec2 velocity, bool isKinematic,
        glm::vec2 acceleration = { 0.0f, 0.0f }, float friction = 5.0f)
        : m_Velocity(velocity), m_IsKinematic(isKinematic),
        m_Acceleration(acceleration), m_Friction(friction)
    {
    }
};


