#include "../pch.h"
#include "CollisionSystem.h"

#include "../Config/GameConfig.h"
#include "../Config/Constants.h"
#include "../ECS/World.h"

// temp hack for score, should be a callback eventually and event
extern int g_GlobalScore;

void CollisionSystem::SetBall(Entity ball)
{
    m_Ball = ball;
}

void CollisionSystem::SetPaddle(Entity paddle)
{
    m_Paddle = paddle;
}

void CollisionSystem::AddBrick(Entity brick)
{
    m_Bricks.push_back(brick);
}

void CollisionSystem::ClearBricks()
{
    m_Bricks.clear();
}

void CollisionSystem::Update(World& world, float deltaTime)
{
    const auto cfg = GameConfig::ForLevel(world.GetCurrentLevel());

    HandleBallBounds(world, cfg);
    HandleBallPaddleCollision(world, cfg);
    HandleBallBrickCollision(world);
}

void CollisionSystem::HandleBallBounds(World& world, const GameConfig::Config& cfg)
{
    if (!world.IsValid(m_Ball) ||
        !world.HasComponent<TransformComponent>(m_Ball) ||
        !world.HasComponent<PhysicsComponent>(m_Ball))
        return;

    auto& transform = world.GetComponent<TransformComponent>(m_Ball);
    auto& physics = world.GetComponent<PhysicsComponent>(m_Ball);

    glm::vec2& pos = transform.m_Position;
    glm::vec2& vel = physics.m_Velocity;
    glm::vec2 size = transform.m_Scale;

    float halfW = size.x * 0.5f;
    float halfH = size.y * 0.5f;

    if (pos.x - halfW < 0.0f) {
        pos.x = halfW;
        vel.x *= -1;
    }
    else if (pos.x + halfW > Constants::ScreenWidth) {
        pos.x = Constants::ScreenWidth - halfW;
        vel.x *= -1;
    }

    if (pos.y + halfH > Constants::ScreenHeight) {
        pos.y = Constants::ScreenHeight - halfH;
        vel.y *= -1;
    }

    float speed = glm::length(vel);
    if (speed > cfg.BallMaxSpeed) {
        vel = glm::normalize(vel) * cfg.BallMaxSpeed;
    }
}

void CollisionSystem::HandleBallBrickCollision(World& world)
{
    if (!world.IsValid(m_Ball)) return;

    auto& ballT = world.GetComponent<TransformComponent>(m_Ball);
    auto& ballP = world.GetComponent<PhysicsComponent>(m_Ball);
    glm::vec2 ballPos = ballT.m_Position;
    glm::vec2 ballHalf = ballT.m_Scale * 0.5f;

    std::vector<Entity> toRemove;

    for (Entity brick : m_Bricks)
    {
        if (!world.IsValid(brick)) continue;

        auto& brickT = world.GetComponent<TransformComponent>(brick);
        glm::vec2 brickPos = brickT.m_Position;
        glm::vec2 brickHalf = brickT.m_Scale * 0.5f;

        bool overlapX = std::abs(ballPos.x - brickPos.x) <= (ballHalf.x + brickHalf.x);
        bool overlapY = std::abs(ballPos.y - brickPos.y) <= (ballHalf.y + brickHalf.y);

        if (overlapX && overlapY)
        {
            glm::vec2 delta = ballPos - brickPos;
            if (std::abs(delta.x) > std::abs(delta.y))
                ballP.m_Velocity.x *= -1;
            else
                ballP.m_Velocity.y *= -1;

            toRemove.push_back(brick);
            g_GlobalScore += 10; // +10 per brick
        }
    }

    for (Entity b : toRemove)
    {
        world.DestroyEntity(b);
        m_Bricks.erase(std::remove(m_Bricks.begin(), m_Bricks.end(), b), m_Bricks.end());
    }
}

void CollisionSystem::HandleBallPaddleCollision(World& world, const GameConfig::Config& cfg)
{
    if (!world.IsValid(m_Ball) || !world.IsValid(m_Paddle))
        return;

    auto& ballT = world.GetComponent<TransformComponent>(m_Ball);
    auto& ballP = world.GetComponent<PhysicsComponent>(m_Ball);

    auto& paddleT = world.GetComponent<TransformComponent>(m_Paddle);

    glm::vec2 ballPos = ballT.m_Position;
    glm::vec2 ballHalf = ballT.m_Scale * 0.5f;

    glm::vec2 paddlePos = paddleT.m_Position;
    glm::vec2 paddleHalf = paddleT.m_Scale * 0.5f;

    bool overlapX = std::abs(ballPos.x - paddlePos.x) <= (ballHalf.x + paddleHalf.x);
    bool overlapY = std::abs(ballPos.y - paddlePos.y) <= (ballHalf.y + paddleHalf.y);

    if (overlapX && overlapY)
    {
        ballT.m_Position.y = paddlePos.y + paddleHalf.y + ballHalf.y;
        ballP.m_Velocity.y = std::abs(ballP.m_Velocity.y);

        float hitPercent = (ballPos.x - paddlePos.x) / paddleHalf.x;
        ballP.m_Velocity.x = hitPercent * cfg.BallInitialVelocity.x;
    }
}
