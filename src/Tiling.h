#ifndef UPT_TILING_H
#define UPT_TILING_H

#include "Arduino.h"
#include "Vector2.h"

namespace sensirion::upt::display {

#define TILE_OFFSET 20
#define TILE_MARGIN 4

/**
 * @brief Represent the size category of a tile
 */
enum struct TileType { UNDEFINED, SMALL, NARROW, MEDIUM, LARGE };

/**
 * @brief Contains the location and size information of a tile
 */
struct SensorDisplayTile {
    Vector2 topLeft;

    uint16_t width;
    uint16_t height;

    TileType type;

    [[nodiscard]] Vector2 getCenter() const {
        return {static_cast<int16_t>(topLeft.x + (width / 2)),
                static_cast<int16_t>(topLeft.y + height / 2)};
    }

    [[nodiscard]] Vector2 getBottomRight() const {
        return {static_cast<int16_t>(topLeft.x + width),
                static_cast<int16_t>(topLeft.y + height)};
    }
};

/**
 * @brief Returns a pointer to a list of N SensorDisplayTile filling the
 * given screen dimensions
 */
SensorDisplayTile* getNTiles(uint16_t numTiles, uint16_t screenWidthPx,
                             uint16_t screenHeightPx);
}  // namespace sensirion::upt::display

#endif /* UPT_TILING_H */