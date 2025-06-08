#pragma once
#include <array>
#include <bitset>
#include <queue>
#include <cassert>

#include "../Core/Types.h"  // defines ComponentSignature

class EntityManager
{
public:
    EntityManager();

    Entity CreateEntity();
    void DestroyEntity(Entity entity);
    void Clear();

    void SetSignature(Entity entity, ComponentSignature signature);
    ComponentSignature GetSignature(Entity entity) const;

private:
    std::queue<Entity> m_AvailableEntities;
    std::array<ComponentSignature, MAX_ENTITIES> m_Signatures;
    uint32_t m_LivingEntityCount = 0;
};
