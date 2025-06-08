#pragma once
#include <set>
#include "../Core/Types.h"

class System
{
public:
    virtual ~System();

    void AddEntity(Entity entity);
    void RemoveEntity(Entity entity);
    std::set<Entity>& GetEntities() { return m_Entities; }
    const std::set<Entity>& GetEntities() const;

protected:
    std::set<Entity> m_Entities;
};
