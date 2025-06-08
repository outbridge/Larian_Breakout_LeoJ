#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <cmath>
#include <algorithm>
#include <typeindex>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <glfw/glfw3.h>

#include "Core/Types.h"
#include "ECS/EntityManager.h"
#include "ECS/ComponentManager.h"
#include "ECS/SystemManager.h"

#include "Components/TransformComponent.h"
#include "Components/RenderableComponent.h"
#include "Components/PhysicsComponent.h"
#include "Components/CharacteristicComponent.h"

#include "Config/Constants.h"
#include "Config/GameConfig.h"
