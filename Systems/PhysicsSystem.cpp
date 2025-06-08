#include "PhysicsSystem.h"
#include "../Config/GameConfig.h"
#include <iostream>

void PhysicsSystem::Update(World& world, float deltaTime, const GameConfig::Config& cfg)
{
    for (Entity entity : System::GetEntities())
    {
        auto& transform = world.GetComponent<TransformComponent>(entity);
        auto& physics = world.GetComponent<PhysicsComponent>(entity);

        if (physics.m_IsKinematic)
        {
            transform.m_Position += physics.m_Velocity * deltaTime;
            continue;
        }

        physics.m_Velocity += physics.m_Acceleration * deltaTime;
        physics.m_Velocity *= (1.0f - physics.m_Friction * deltaTime);

        float speedSq = glm::dot(physics.m_Velocity, physics.m_Velocity);
        float maxSpeedSq = cfg.BallMaxSpeed * cfg.BallMaxSpeed;
        if (speedSq > maxSpeedSq) {
            physics.m_Velocity = glm::normalize(physics.m_Velocity) * cfg.BallMaxSpeed;
        }

        transform.m_Position += physics.m_Velocity * deltaTime;

        //std::cout << "[PhysicsSystem] Entity " << entity
        //    << " | Vel: " << physics.m_Velocity.x
        //    << ", " << physics.m_Velocity.y
        //    << " | Pos: " << transform.m_Position.x
        //    << ", " << transform.m_Position.y << "\n";
    }
}
