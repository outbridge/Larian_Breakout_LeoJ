#pragma once

#include <vector>
#include "../ECS/System.h"
#include "../Core/Types.h"
#include "../Components/TransformComponent.h"
#include "../Components/PhysicsComponent.h"
#include "../Components/CharacteristicComponent.h"
#include "../Config/GameConfig.h"

class World;

class CollisionSystem : public System
{
public:
    void SetBall(Entity ball);
    void SetPaddle(Entity paddle);
    void AddBrick(Entity brick);
    void ClearBricks();

    void Update(World& world, float deltaTime);

private:
    void HandleBallBounds(World& world, const GameConfig::Config& cfg);
    void HandleBallBrickCollision(World& world);
    void HandleBallPaddleCollision(World& world, const GameConfig::Config& cfg);

    Entity m_Ball = 0;
    Entity m_Paddle = 0;
    std::vector<Entity> m_Bricks;
};
