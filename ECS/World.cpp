#include "World.h"

World::World()
{
    m_ComponentManager = std::make_unique<ComponentManager>();
    m_EntityManager = std::make_unique<EntityManager>();
    m_SystemManager = std::make_unique<SystemManager>();
}

void World::Clear()
{
    m_ComponentManager->Clear();
    m_EntityManager->Clear();
    m_SystemManager->Clear();
}

Entity World::CreateEntity()
{
    return m_EntityManager->CreateEntity();
}

void World::DestroyEntity(Entity entity)
{
    m_EntityManager->DestroyEntity(entity);
    m_ComponentManager->EntityDestroyed(entity);
    m_SystemManager->EntityDestroyed(entity);
}

void World::EntitySignatureChanged(Entity entity, const ComponentSignature& signature)
{
    m_SystemManager->EntitySignatureChanged(entity, signature);
}

bool World::IsValid(Entity entity) const
{
    if (entity >= MAX_ENTITIES) return false;
    const auto& signature = m_EntityManager->GetSignature(entity);

    return signature.any();
}


ComponentSignature World::GetSignature(Entity entity) const
{
    return m_EntityManager->GetSignature(entity);
}

std::vector<Entity> World::GetAllEntities() const
{
    std::vector<Entity> entities;
    for (Entity entity = 0; entity < MAX_ENTITIES; ++entity)
    {
        if (IsValid(entity))  // Assumes you already have IsValid() implemented
            entities.push_back(entity);
    }
    return entities;
}


