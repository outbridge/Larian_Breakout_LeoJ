#include "SystemManager.h"
#include <iostream>

void SystemManager::EntityDestroyed(Entity entity)
{
    for (auto& [_, system] : m_Systems)
    {
        system->GetEntities().erase(entity);
    }
}

void SystemManager::EntitySignatureChanged(Entity entity, const ComponentSignature& entitySignature)
{
    for (auto const& [type, system] : m_Systems)
    {
        const auto& systemSignature = m_Signatures[type];

        if ((entitySignature & systemSignature) == systemSignature)
        {
            system->GetEntities().insert(entity);
        }
        else
        {
            system->GetEntities().erase(entity);
        }
    }
}

void SystemManager::Clear()
{
    m_Systems.clear();
    m_Signatures.clear();
}

