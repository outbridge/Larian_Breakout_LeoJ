#pragma once
#include <bitset>
#include <cstdint>

constexpr std::size_t MAX_ENTITIES = 5000;
constexpr std::size_t MAX_COMPONENTS = 64;

using Entity = std::uint32_t;
using ComponentType = std::uint8_t;
using ComponentSignature = std::bitset<MAX_COMPONENTS>;
