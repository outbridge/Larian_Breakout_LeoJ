#include "GameStateManager.h"

void GameStateManager::PushState(std::unique_ptr<GameState> state)
{
    m_States.push(std::move(state));
}

void GameStateManager::PopState()
{
    if (!m_States.empty())
    {
        m_States.pop();
    }
}

void GameStateManager::ChangeState(std::unique_ptr<GameState> state)
{
    if (!m_States.empty())
    {
        m_States.pop();
    }
    m_States.push(std::move(state));
}

GameState* GameStateManager::GetActiveState()
{
    if (!m_States.empty())
    {
        return m_States.top().get();
    }
    return nullptr;
}
