#include "../pch.h"
#include "../Level/LevelManager.h"

#include "../ECS/World.h"
#include "../Components/TransformComponent.h"
#include "../Components/RenderableComponent.h"
#include "../Components/CharacteristicComponent.h"
#include "../Systems/CollisionSystem.h"

#include <algorithm>

void LevelManager::LoadLevel(int index, World& world)
{
    const auto cfg = GameConfig::ForLevel(index);

    const float availableWidth = Constants::ScreenWidth - cfg.BrickMarginLeft - cfg.BrickMarginRight;
    const float availableHeight = Constants::ScreenHeight - cfg.BrickMarginTop - cfg.BrickMarginBottom;

    const float totalBrickWidth = cfg.BrickSize.x + cfg.BrickPadding;
    const float totalBrickHeight = cfg.BrickSize.y + cfg.BrickPadding;

    const int cols = std::max(1, static_cast<int>(availableWidth / totalBrickWidth));
    const int rows = std::min(cfg.BrickRows, static_cast<int>(availableHeight / totalBrickHeight));

    const float startX = cfg.BrickMarginLeft + cfg.BrickSize.x * 0.5f;
    const float startY = Constants::ScreenHeight - cfg.BrickMarginTop - cfg.BrickSize.y * 0.5f;

    for (int row = 0; row < rows; ++row)
    {
        for (int col = 0; col < cols; ++col)
        {
            float x = startX + col * totalBrickWidth;
            float y = startY - row * totalBrickHeight;

            Entity brick = world.CreateEntity();

            TransformComponent transform{ glm::vec2(x, y), cfg.BrickSize };
            RenderableComponent renderable{ cfg.BrickColor };
            CharacteristicComponent characteristic{ ObjectRole::Brick };

            world.AddComponent(brick, transform);
            world.AddComponent(brick, renderable);
            world.AddComponent(brick, characteristic);

            ComponentSignature sig;
            sig.set(world.GetComponentType<TransformComponent>());
            sig.set(world.GetComponentType<RenderableComponent>());
            sig.set(world.GetComponentType<CharacteristicComponent>());
            world.EntitySignatureChanged(brick, sig);

            if (auto collision = world.GetSystem<CollisionSystem>())
                collision->AddBrick(brick);
        }
    }
}

bool LevelManager::IsLevelComplete(const World& world) const
{
    for (Entity entity : world.GetAllEntities())
    {
        if (!world.IsValid(entity)) continue;

        if (!world.HasComponent<CharacteristicComponent>(entity)) continue;

        const auto& comp = world.GetComponent<CharacteristicComponent>(entity);
        if (comp.Role == ObjectRole::Brick)
            return false;
    }
    return true;
}
