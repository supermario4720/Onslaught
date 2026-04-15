#pragma once
#include <cstdint>

// separate file for ObjectID for easier access accross files
enum class ObjectID : std::uint16_t {
    None = 0,
    Tree,
    Stone,
    COUNT
};
