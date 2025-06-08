#include "ComponentManager.h"

void ComponentManager::Clear()
{
    m_ComponentTypes.clear();
    m_ComponentArrays.clear();
    m_NextComponentType = 0;
}

