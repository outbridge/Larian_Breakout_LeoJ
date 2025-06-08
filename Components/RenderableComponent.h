#pragma once

#include <glm/glm.hpp>

struct RenderableComponent
{
    glm::vec3 m_Color = { 1.0f, 1.0f, 1.0f }; // RGB
    // texture ID, shader info, layer, etc.
};
