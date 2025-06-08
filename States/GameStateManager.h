#pragma once

#include <memory>
#include <stack>
#include "GameState.h"

class GameStateManager
{
public:
    void PushState(std::unique_ptr<GameState> state);
    void PopState();
    void ChangeState(std::unique_ptr<GameState> state);
    GameState* GetActiveState();

private:
    std::stack<std::unique_ptr<GameState>> m_States;
};
