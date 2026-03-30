#pragma once
#include <cstdint>

// separate file for ItemID for easier usage
enum class ItemID : uint16_t {
    None = 0,
    Wood,
    Stone,
    COUNT
};