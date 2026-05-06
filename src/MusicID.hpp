#pragma once
#include <cstdint>

enum class MusicID : std::uint16_t {
    None = 0,
    Title,
    Game,
    Wave,
    Victory,
    GameOver,
    COUNT
};
