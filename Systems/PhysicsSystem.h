#pragma once

#include "../Config/GameConfig.h"
#include "../ECS/System.h"
#include "../ECS/World.h"
#include "../Components/TransformComponent.h"
#include "../Components/PhysicsComponent.h"

class PhysicsSystem : public System
{
public:
    void Update(World& world, float deltaTime, const GameConfig::Config& cfg);
};
