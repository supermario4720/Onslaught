#pragma once
#include <cstdint>


enum class BuildingID : uint16_t {
    None = 0,
    ArcherTower,
    Wall,
    COUNT
};

// get the next building ID in the list
inline BuildingID nextBuildingID(BuildingID current) {
    using underlying = std::underlying_type_t<BuildingID>;
    underlying value = static_cast<underlying>(current);
    underlying max   = static_cast<underlying>(BuildingID::COUNT) - 1;

    if (value >= max) {
        return static_cast<BuildingID>(1); // skip None
    }
    return static_cast<BuildingID>(value + 1);
}
