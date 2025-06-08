#pragma once

class GameState
{
public:
    virtual ~GameState() = default;

    virtual void HandleInput() = 0;
    virtual void Update(float deltaTime) = 0;
    virtual void Render() = 0;
};
