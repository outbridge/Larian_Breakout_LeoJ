#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <typeinfo>
#include <cassert>
#include <typeindex>

#include "../Core/Types.h"
#include "System.h"

class SystemManager
{
public:
    void Clear();

    template<typename T>
    std::shared_ptr<T> RegisterSystem()
    {
        std::type_index typeName = typeid(T);
        assert(m_Systems.find(typeName) == m_Systems.end() && "System already registered.");

        auto system = std::make_shared<T>();
        m_Systems[typeName] = system;
        return system;
    }

    template<typename T>
    std::shared_ptr<T> GetSystem()
    {
        std::type_index typeName = typeid(T);
        assert(m_Systems.find(typeName) != m_Systems.end() && "System not registered.");

        return std::static_pointer_cast<T>(m_Systems.at(typeName));
    }

    template<typename T>
    std::shared_ptr<T> GetSystem() const
    {
        std::type_index typeName = typeid(T);
        assert(m_Systems.find(typeName) != m_Systems.end() && "System not registered.");

        return std::static_pointer_cast<T>(m_Systems.at(typeName));
    }

    template<typename T>
    void SetSignature(ComponentSignature signature)
    {
        std::type_index typeName = typeid(T);
        assert(m_Systems.find(typeName) != m_Systems.end() && "System not registered.");

        m_Signatures[typeName] = signature;
    }

    void EntityDestroyed(Entity entity);
    void EntitySignatureChanged(Entity entity, const ComponentSignature& entitySignature);

private:
    std::unordered_map<std::type_index, std::shared_ptr<System>> m_Systems;
    std::unordered_map<std::type_index, ComponentSignature> m_Signatures;
};
