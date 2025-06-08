#include "EntityManager.h"

EntityManager::EntityManager()
{
    for (Entity entity = 0; entity < MAX_ENTITIES; ++entity)
        m_AvailableEntities.push(entity);
}

Entity EntityManager::CreateEntity()
{
    assert(m_LivingEntityCount < MAX_ENTITIES && "Too many entities in use.");
    Entity id = m_AvailableEntities.front();
    m_AvailableEntities.pop();
    ++m_LivingEntityCount;
    return id;
}

void EntityManager::DestroyEntity(Entity entity)
{
    assert(entity < MAX_ENTITIES && "Entity out of range.");
    m_Signatures[entity].reset();
    m_AvailableEntities.push(entity);
    --m_LivingEntityCount;
}

void EntityManager::Clear()
{
    while (!m_AvailableEntities.empty())
        m_AvailableEntities.pop();

    for (Entity entity = 0; entity < MAX_ENTITIES; ++entity)
        m_AvailableEntities.push(entity);

    m_Signatures.fill(ComponentSignature{});
    m_LivingEntityCount = 0;
}

void EntityManager::SetSignature(Entity entity, ComponentSignature signature)
{
    assert(entity < MAX_ENTITIES && "Entity out of range.");
    m_Signatures[entity] = signature;
}

ComponentSignature EntityManager::GetSignature(Entity entity) const
{
    assert(entity < MAX_ENTITIES && "Entity out of range.");
    return m_Signatures[entity];
}
