#include "System.h"

System::~System() = default;

void System::AddEntity(Entity entity)
{
    m_Entities.insert(entity);
}

void System::RemoveEntity(Entity entity)
{
    m_Entities.erase(entity);
}

const std::set<Entity>& System::GetEntities() const
{
    return m_Entities;
}