#ifndef _TILING_H_
#define _TILING_H_

#include "Arduino.h"

#define TILE_BOTTOM_OFFSET 20
#define TILE_VERTICAL_TOP_OFFSET 20
#define INTER_TILE_SPACING 4

/**
 * @brief Represent the size category of a tile
 */
enum struct TileType { UNDEFINED, SMALL, NARROW, MEDIUM, LARGE };

/**
 * @brief Contains the location and size information of a tile
 */
struct SensorDisplayTile {
    uint tlx;
    uint tly;
    uint brx;
    uint bry;
    TileType type;

    uint getCx() {
        return (brx + tlx) / 2;
    }

    uint getCy() {
        return (bry + tly) / 2;
    }

    uint getHeight() {
        return bry - tly;
    }

    uint getWidth() {
        return brx - tlx;
    }
};

/**
 * @brief Returns a pointer to a list of N SensorDisplayTile filling the
 * given screen dimensions
 */
SensorDisplayTile* getNTiles(uint n, uint screen_width_px,
                             uint screen_height_px);

SensorDisplayTile* _get1Tile(uint screen_width_px, uint screen_height_px);
SensorDisplayTile* _get2Tile(uint screen_width_px, uint screen_height_px);
SensorDisplayTile* _get3Tile(uint screen_width_px, uint screen_height_px);
SensorDisplayTile* _get4Tile(uint screen_width_px, uint screen_height_px);
SensorDisplayTile* _get5Tile(uint screen_width_px, uint screen_height_px);
SensorDisplayTile* _get6Tile(uint screen_width_px, uint screen_height_px);
SensorDisplayTile* _get7Tile(uint screen_width_px, uint screen_height_px);
SensorDisplayTile* _get8Tile(uint screen_width_px, uint screen_height_px);

SensorDisplayTile* _get1VTile(uint screen_width_px, uint screen_height_px);
SensorDisplayTile* _get2VTile(uint screen_width_px, uint screen_height_px);
SensorDisplayTile* _get3VTile(uint screen_width_px, uint screen_height_px);
SensorDisplayTile* _get4VTile(uint screen_width_px, uint screen_height_px);
SensorDisplayTile* _get5VTile(uint screen_width_px, uint screen_height_px);
SensorDisplayTile* _get6VTile(uint screen_width_px, uint screen_height_px);

#endif /* _UPT_TILING_H_ */