#pragma once

#include "../Config/GameConfig.h"
#include "../Core/Types.h"
#include "../ECS/World.h"
#include "../States/GameState.h"

#include <memory>

class GLFWwindow;
class EntityManager;
class LevelManager;
class RenderSystem;
class CollisionSystem;

class PlayingState : public GameState
{
public:
    explicit PlayingState(GLFWwindow* window);

    void HandleInput() override;
    void Update(float deltaTime) override;
    void Render() override;

private:
    GLFWwindow* m_Window = nullptr;
    GameConfig::Config m_Config;

    std::unique_ptr<EntityManager> m_EntityManager;
    std::unique_ptr<LevelManager> m_LevelManager;
    World m_World;

    std::shared_ptr<RenderSystem> m_RenderSystem;
    std::shared_ptr<CollisionSystem> m_CollisionSystem;

    Entity m_Paddle;
    Entity m_Ball;

    bool m_BallLaunched = false;
    bool m_GameWon = false;
    bool m_GameOver = false;
    bool m_LevelComplete = false;
    int m_CurrentLevel = 0;
    int m_Score = 0;

    void CreatePaddle();
    void CreateBall();
    void RegisterComponentsAndSystems();
    void StartNextLevel();
    void ResetGameState();
};
