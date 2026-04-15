#include "ItemData.hpp"

const ItemData& ItemDatabase::get(ItemID id) {
    return getMap().at(id);
}

const std::unordered_map<ItemID, ItemData>& ItemDatabase::getMap() {
    static const std::unordered_map<ItemID, ItemData> map = {
        { ItemID::None, {
            "None",
            "No Description",
            false,
            1
        }},

        { ItemID::Wood, {
            "Wood",
            "Basic material for construction.",
            true,
            99
        }},

        { ItemID::Stone, {
            "Stone",
            "Basic material for construction.",
            true,
            99
        }},
    };
    return map;
}
