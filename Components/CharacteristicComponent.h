#pragma once

enum class ObjectRole
{
    None,
    Ball,
    Paddle,
    Brick,
    PowerUp
};

struct CharacteristicComponent
{
    ObjectRole Role = ObjectRole::None;

    // optional bit flags for finer-grain classification later
    uint32_t Flags = 0;
};
