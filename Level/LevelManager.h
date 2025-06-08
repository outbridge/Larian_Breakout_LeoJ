#pragma once

#include "../Core/Types.h"

class World;

class LevelManager
{
public:
    void LoadLevel(int index, World& world);
    bool IsLevelComplete(const World& world) const;

private:
    // store current level layout or metadata here if needed later
};
