#include "../pch.h"
#include "PlayingState.h"

#include "../Systems/RenderSystem.h"
#include "../Systems/CollisionSystem.h"
#include "../Systems/PhysicsSystem.h"
#include "../Level/LevelManager.h"
#include "../Config/GameConfig.h"

int g_GlobalScore = 0;

PlayingState::PlayingState(GLFWwindow* window)
    : m_Window(window)
{
    m_EntityManager = std::make_unique<EntityManager>();
    std::cout << "[PlayingState] Initialized.\n";

    m_LevelManager = std::make_unique<LevelManager>();
    RegisterComponentsAndSystems();

    m_CurrentLevel = 0;
    m_Config = GameConfig::ForLevel(m_CurrentLevel);

    m_LevelManager->LoadLevel(m_CurrentLevel, m_World);
    CreatePaddle();
    CreateBall();
}

void PlayingState::HandleInput()
{
    if (m_GameOver)
    {
        if (glfwGetKey(m_Window, GLFW_KEY_R) == GLFW_PRESS)
        {
            std::cout << "[Game] Restarting...\n";
            ResetGameState();
        }
        return;
    }

    if (m_GameWon || m_GameOver) 
        return;

    // Launch ball with space
    if (!m_BallLaunched)
    {
        auto& ballPhysics = m_World.GetComponent<PhysicsComponent>(m_Ball);
        auto& paddleTransform = m_World.GetComponent<TransformComponent>(m_Paddle);
        auto& ballTransform = m_World.GetComponent<TransformComponent>(m_Ball);

        ballTransform.m_Position = paddleTransform.m_Position + glm::vec2(0.0f, m_Config.PaddleSize.y / 2 + m_Config.BallSize.y);

        if (glfwGetKey(m_Window, GLFW_KEY_SPACE) == GLFW_PRESS)
        {
            ballPhysics.m_Velocity = m_Config.BallInitialVelocity;
            m_BallLaunched = true;
        }
    }

    // Paddle movement
    auto& physics = m_World.GetComponent<PhysicsComponent>(m_Paddle);

    if (glfwGetKey(m_Window, Constants::KeyLeft) == GLFW_PRESS)
        physics.m_Velocity.x = -m_Config.PaddleSpeed;
    else if (glfwGetKey(m_Window, Constants::KeyRight) == GLFW_PRESS)
        physics.m_Velocity.x = m_Config.PaddleSpeed;
    else
        physics.m_Velocity.x = 0.0f;

    auto& transform = m_World.GetComponent<TransformComponent>(m_Paddle);
    float halfWidth = m_Config.PaddleSize.x / 2.0f;
    transform.m_Position.x = glm::clamp(transform.m_Position.x, halfWidth, Constants::ScreenWidth - halfWidth);
}

void PlayingState::Update(float deltaTime)
{
    m_Score = g_GlobalScore;

    if (m_GameWon || m_GameOver) return;

    const auto cfg = GameConfig::ForLevel(m_CurrentLevel);

    m_World.GetSystem<PhysicsSystem>()->Update(m_World, deltaTime, cfg);
    m_CollisionSystem->Update(m_World, deltaTime);

    auto& ballTransform = m_World.GetComponent<TransformComponent>(m_Ball);
    if (ballTransform.m_Position.y < 0.0f)
    {
        std::cout << "[Game] You lost!\n";
        m_GameOver = true;
        return;
    }

    if (!m_LevelComplete && m_LevelManager->IsLevelComplete(m_World))
    {
        std::cout << "[Game] Level " << m_CurrentLevel << " complete!\n";
        m_LevelComplete = true;
        StartNextLevel();
    }
}

void PlayingState::StartNextLevel()
{
    m_CurrentLevel++;
    m_World.Clear();

    m_Paddle = 0;
    m_Ball = 0;
    m_Config = GameConfig::ForLevel(m_CurrentLevel);

    RegisterComponentsAndSystems();
    m_LevelManager->LoadLevel(m_CurrentLevel, m_World);
    CreatePaddle();
    CreateBall();

    m_World.SetCurrentLevel(m_CurrentLevel);

    m_BallLaunched = false;
    m_GameOver = false;
    m_LevelComplete = false;
}

void PlayingState::RegisterComponentsAndSystems()
{
    m_World.RegisterComponent<TransformComponent>();
    m_World.RegisterComponent<PhysicsComponent>();
    m_World.RegisterComponent<RenderableComponent>();
    m_World.RegisterComponent<CharacteristicComponent>();

    auto physicsSystem = m_World.RegisterSystem<PhysicsSystem>();
    ComponentSignature physicsSig;
    physicsSig.set(m_World.GetComponentType<TransformComponent>());
    physicsSig.set(m_World.GetComponentType<PhysicsComponent>());
    m_World.SetSystemSignature<PhysicsSystem>(physicsSig);

    auto renderSystem = m_World.RegisterSystem<RenderSystem>();
    renderSystem->Init();
    ComponentSignature renderSig;
    renderSig.set(m_World.GetComponentType<TransformComponent>());
    renderSig.set(m_World.GetComponentType<RenderableComponent>());
    m_World.SetSystemSignature<RenderSystem>(renderSig);
    m_RenderSystem = renderSystem;

    auto collisionSystem = m_World.RegisterSystem<CollisionSystem>();
    ComponentSignature collisionSig;
    collisionSig.set(m_World.GetComponentType<TransformComponent>());
    collisionSig.set(m_World.GetComponentType<PhysicsComponent>());
    m_World.SetSystemSignature<CollisionSystem>(collisionSig);
    m_CollisionSystem = collisionSystem;
}

void PlayingState::Render()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    m_RenderSystem->Update(m_World);

    std::string title;

    if (m_GameOver)
        title = "You lost! Press R to restart | Score: " + std::to_string(m_Score);
    else if (!m_BallLaunched)
        title = "Press SPACE to launch, LEFT/RIGHT to move | Score: " + std::to_string(m_Score);
    else
        title = "Breakout | Score: " + std::to_string(m_Score);

    glfwSetWindowTitle(m_Window, title.c_str());
}

void PlayingState::CreatePaddle()
{
    Entity paddle = m_World.CreateEntity();

    TransformComponent transform{ m_Config.PaddleStartPos, m_Config.PaddleSize };
    PhysicsComponent physics{ glm::vec2(0.0f), true, glm::vec2(0.0f), 10.0f };
    RenderableComponent renderable{ m_Config.PaddleColor };

    m_World.AddComponent(paddle, transform);
    m_World.AddComponent(paddle, physics);
    m_World.AddComponent(paddle, renderable);

    ComponentSignature sig;
    sig.set(m_World.GetComponentType<TransformComponent>());
    sig.set(m_World.GetComponentType<PhysicsComponent>());
    sig.set(m_World.GetComponentType<RenderableComponent>());
    m_World.EntitySignatureChanged(paddle, sig);

    m_Paddle = paddle;
    m_CollisionSystem->SetPaddle(paddle);
}

void PlayingState::CreateBall()
{
    Entity ball = m_World.CreateEntity();
    auto& paddleTransform = m_World.GetComponent<TransformComponent>(m_Paddle);

    TransformComponent transform;
    transform.m_Position = paddleTransform.m_Position + glm::vec2(0.0f, m_Config.PaddleSize.y / 2 + m_Config.BallSize.y);
    transform.m_Scale = m_Config.BallSize;

    PhysicsComponent physics{ m_Config.BallInitialVelocity, false, glm::vec2(0.0f), 0.0f };
    RenderableComponent renderable{ m_Config.BallColor };
    CharacteristicComponent characteristic{ ObjectRole::Ball };

    m_World.AddComponent(ball, transform);
    m_World.AddComponent(ball, physics);
    m_World.AddComponent(ball, renderable);
    m_World.AddComponent(ball, characteristic);

    ComponentSignature sig;
    sig.set(m_World.GetComponentType<TransformComponent>());
    sig.set(m_World.GetComponentType<PhysicsComponent>());
    sig.set(m_World.GetComponentType<RenderableComponent>());
    sig.set(m_World.GetComponentType<CharacteristicComponent>());
    m_World.EntitySignatureChanged(ball, sig);

    m_Ball = ball;
    m_BallLaunched = false;
    m_CollisionSystem->SetBall(ball);
}

void PlayingState::ResetGameState()
{
    m_Paddle = 0;
    m_Ball = 0;
    m_BallLaunched = false;
    m_GameOver = false;
    m_GameWon = false;
    m_LevelComplete = false;
    m_CurrentLevel = 0;
    m_Score = 0;

    m_World.Clear();
    RegisterComponentsAndSystems();
    m_LevelManager->LoadLevel(m_CurrentLevel, m_World);
    CreatePaddle();
    CreateBall();
}
