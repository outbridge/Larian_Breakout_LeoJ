#pragma once

#include "../Core/Types.h"

#include "ComponentManager.h"
#include "EntityManager.h"
#include "SystemManager.h"
#include <memory>
#include <unordered_map>
#include <cassert>

class World
{
public:
    World();

    Entity CreateEntity();
    void Clear();
    void DestroyEntity(Entity entity);

    template<typename T>
    void RegisterComponent()
    {
        m_ComponentManager->RegisterComponent<T>();
    }

    template<typename T>
    void AddComponent(Entity entity, T component)
    {
        m_ComponentManager->AddComponent<T>(entity, component);

        auto signature = m_EntityManager->GetSignature(entity);
        signature.set(m_ComponentManager->GetComponentType<T>());
        m_EntityManager->SetSignature(entity, signature);

        EntitySignatureChanged(entity, signature);
    }

    template<typename T>
    void RemoveComponent(Entity entity)
    {
        m_ComponentManager->RemoveComponent<T>(entity);

        ComponentSignature signature = m_EntityManager->GetSignature(entity);
        signature.reset(m_ComponentManager->GetComponentType<T>());
        m_EntityManager->SetSignature(entity, signature);

        m_SystemManager->EntitySignatureChanged(entity, signature);
    }

    template <typename T>
    bool HasComponent(Entity entity) const
    {
        return m_ComponentManager->HasComponent<T>(entity);
    }

    template<typename T>
    T& GetComponent(Entity entity)
    {
        return m_ComponentManager->GetComponent<T>(entity);
    }

    template<typename T>
    const T& GetComponent(Entity entity) const
    {
        return m_ComponentManager->GetComponent<T>(entity);
    }

    template<typename T>
    ComponentType GetComponentType() const
    {
        return m_ComponentManager->GetComponentType<T>();
    }


    template<typename T>
    std::shared_ptr<T> RegisterSystem()
    {
        return m_SystemManager->RegisterSystem<T>();
    }

    template<typename T>
    void SetSystemSignature(ComponentSignature signature)
    {
        m_SystemManager->SetSignature<T>(signature);
    }

    template<typename T>
    std::shared_ptr<T> GetSystem()
    {
        return m_SystemManager->GetSystem<T>();
    }

    template<typename T>
    std::shared_ptr<T> GetSystem() const
    {
        return m_SystemManager->GetSystem<T>();
    }

    void EntitySignatureChanged(Entity entity, const ComponentSignature& signature);
    bool IsValid(Entity entity) const;
    ComponentSignature GetSignature(Entity entity) const;
    std::vector<Entity> GetAllEntities() const;
    int GetCurrentLevel() const { return m_CurrentLevel; }
    void SetCurrentLevel(int level) { m_CurrentLevel = level; }

private:
    std::unique_ptr<ComponentManager> m_ComponentManager;
    std::unique_ptr<EntityManager> m_EntityManager;
    std::unique_ptr<SystemManager> m_SystemManager;

    int m_CurrentLevel = 0;
};
