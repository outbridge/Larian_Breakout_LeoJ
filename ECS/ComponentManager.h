#pragma once

#include <unordered_map>
#include <memory>
#include <typeindex>
#include <type_traits>
#include <cassert>

#include "../Core/Types.h"


class IComponentArray
{
public:
    virtual ~IComponentArray() = default;
    virtual void EntityDestroyed(Entity entity) = 0;
    virtual bool Has(Entity entity) const = 0;
};

template<typename T>
class ComponentArray : public IComponentArray
{
public:
    void InsertData(Entity entity, T component)
    {
        assert(m_EntityToIndexMap.find(entity) == m_EntityToIndexMap.end() && "Component added twice to same entity.");

        m_EntityToIndexMap[entity] = component;
    }

    void RemoveData(Entity entity)
    {
        assert(m_EntityToIndexMap.find(entity) != m_EntityToIndexMap.end() && "Removing non-existent component.");

        m_EntityToIndexMap.erase(entity);
    }

    T& GetData(Entity entity)
    {
        assert(m_EntityToIndexMap.find(entity) != m_EntityToIndexMap.end() && "Component not found on entity.");
        return m_EntityToIndexMap[entity];
    }

    void EntityDestroyed(Entity entity) override
    {
        m_EntityToIndexMap.erase(entity);
    }

    bool Has(Entity entity) const override
    {
        return m_EntityToIndexMap.find(entity) != m_EntityToIndexMap.end();
    }

private:
    std::unordered_map<Entity, T> m_EntityToIndexMap;
};

class ComponentManager
{
public:
    void Clear();

    template<typename T>
    void RegisterComponent()
    {
        std::type_index typeName = typeid(T);
        assert(m_ComponentTypes.find(typeName) == m_ComponentTypes.end() && "Component already registered.");

        m_ComponentTypes[typeName] = m_NextComponentType;
        m_ComponentArrays[typeName] = std::make_unique<ComponentArray<T>>();
        ++m_NextComponentType;
    }

    template<typename T>
    ComponentType GetComponentType()
    {
        std::type_index typeName = typeid(T);
        assert(m_ComponentTypes.find(typeName) != m_ComponentTypes.end() && "Component not registered.");
        return m_ComponentTypes[typeName];
    }

    template <typename T>
    bool HasComponent(Entity entity)
    {
        std::type_index typeIdx = typeid(T);
        auto it = m_ComponentArrays.find(typeIdx);
        return it != m_ComponentArrays.end() && it->second->Has(entity);
    }


    template<typename T>
    void AddComponent(Entity entity, T component)
    {
        GetComponentArray<T>()->InsertData(entity, component);
    }

    template<typename T>
    void RemoveComponent(Entity entity)
    {
        GetComponentArray<T>()->RemoveData(entity);
    }

    template<typename T>
    T& GetComponent(Entity entity)
    {
        return GetComponentArray<T>()->GetData(entity);
    }

    template<typename T>
    const T& GetComponent(Entity entity) const
    {
        ComponentType type = GetComponentType<T>();
        return std::static_pointer_cast<ComponentArray<T>>(m_ComponentArrays.at(type))->Get(entity);
    }

    void EntityDestroyed(Entity entity)
    {
        for (auto const& [type, array] : m_ComponentArrays)
        {
            array->EntityDestroyed(entity);
        }
    }

private:
    std::unordered_map<std::type_index, ComponentType> m_ComponentTypes;
    std::unordered_map<std::type_index, std::unique_ptr<IComponentArray>> m_ComponentArrays;
    ComponentType m_NextComponentType = 0;

    template<typename T>
    ComponentArray<T>* GetComponentArray()
    {
        std::type_index typeName = typeid(T);
        assert(m_ComponentTypes.find(typeName) != m_ComponentTypes.end() && "Component not registered.");
        return static_cast<ComponentArray<T>*>(m_ComponentArrays[typeName].get());
    }
};
