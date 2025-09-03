#include "Tiling.h"

namespace sensirion::upt::display {

constexpr auto TAG = "VIZ";

uint8_t getMaxNTiles(uint screen_width_px, uint screen_height_px) {
    if (screen_height_px <= screen_width_px) {
        // Horizontal layout
        return 8;
    } else {
        // Vertical layout
        return 6;
    }
}

SensorDisplayTile* getNTiles(uint n, uint screen_width_px,
                             uint screen_height_px) {

    // Check if request if possible to satisfy
    if (n > getMaxNTiles(screen_width_px, screen_height_px)) {
        ESP_LOGE(TAG, "Requested too many tile for the current configuration");
        return nullptr;
    }

    if (screen_height_px <= screen_width_px) {
        // Horizontal layout
        switch (n) {
            case 1:
                return _get1Tile(screen_width_px, screen_height_px);
            case 2:
                return _get2Tile(screen_width_px, screen_height_px);
            case 3:
                return _get3Tile(screen_width_px, screen_height_px);
            case 4:
                return _get4Tile(screen_width_px, screen_height_px);
            case 5:
                return _get5Tile(screen_width_px, screen_height_px);
            case 6:
                return _get6Tile(screen_width_px, screen_height_px);
            case 7:
                return _get7Tile(screen_width_px, screen_height_px);
            case 8:
                return _get8Tile(screen_width_px, screen_height_px);

            default:
                return nullptr;
        }
    } else {
        // Vertical layout
        switch (n) {
            case 1:
                return _get1VTile(screen_width_px, screen_height_px);
            case 2:
                return _get2VTile(screen_width_px, screen_height_px);
            case 3:
                return _get3VTile(screen_width_px, screen_height_px);
            case 4:
                return _get4VTile(screen_width_px, screen_height_px);
            case 5:
                return _get5VTile(screen_width_px, screen_height_px);
            case 6:
                return _get6VTile(screen_width_px, screen_height_px);

            default:
                return nullptr;
        }
    }
}

SensorDisplayTile* _get1Tile(uint screen_width_px, uint screen_height_px) {
    SensorDisplayTile* tiles = new SensorDisplayTile[1];

    tiles[0] = {INTER_TILE_SPACING, INTER_TILE_SPACING,
                screen_width_px - INTER_TILE_SPACING,
                screen_height_px - TILE_BOTTOM_OFFSET, TileType::LARGE};
    return tiles;
}

SensorDisplayTile* _get2Tile(uint screen_width_px, uint screen_height_px) {
    SensorDisplayTile* tiles = new SensorDisplayTile[2];

    uint canvasCenterLineX = screen_width_px / 2;

    tiles[0] = {INTER_TILE_SPACING, INTER_TILE_SPACING,
                canvasCenterLineX - INTER_TILE_SPACING / 2,
                screen_height_px - TILE_BOTTOM_OFFSET, TileType::MEDIUM};

    tiles[1] = {canvasCenterLineX + INTER_TILE_SPACING / 2, INTER_TILE_SPACING,
                screen_width_px - INTER_TILE_SPACING,
                screen_height_px - TILE_BOTTOM_OFFSET, TileType::MEDIUM};

    return tiles;
}

SensorDisplayTile* _get3Tile(uint screen_width_px, uint screen_height_px) {
    SensorDisplayTile* tiles = new SensorDisplayTile[3];

    uint canvasCenterLineX = screen_width_px / 2;
    uint canvasCenterLineY = (screen_height_px - TILE_BOTTOM_OFFSET) / 2;

    // Top tile
    tiles[0] = {INTER_TILE_SPACING, INTER_TILE_SPACING,
                screen_width_px - INTER_TILE_SPACING,
                canvasCenterLineY - INTER_TILE_SPACING / 2, TileType::LARGE};

    // Bottom row
    tiles[1] = {INTER_TILE_SPACING, canvasCenterLineY + INTER_TILE_SPACING / 2,
                canvasCenterLineX - INTER_TILE_SPACING / 2,
                screen_height_px - TILE_BOTTOM_OFFSET, TileType::MEDIUM};

    tiles[2] = {canvasCenterLineX + INTER_TILE_SPACING / 2,
                canvasCenterLineY + INTER_TILE_SPACING / 2,
                screen_width_px - INTER_TILE_SPACING,
                screen_height_px - TILE_BOTTOM_OFFSET, TileType::MEDIUM};

    return tiles;
}

SensorDisplayTile* _get4Tile(uint screen_width_px, uint screen_height_px) {
    SensorDisplayTile* tiles = new SensorDisplayTile[4];

    uint canvasCenterLineX = screen_width_px / 2;
    uint canvasCenterLineY = (screen_height_px - TILE_BOTTOM_OFFSET) / 2;

    // Top row
    tiles[0] = {INTER_TILE_SPACING, INTER_TILE_SPACING,
                canvasCenterLineX - INTER_TILE_SPACING / 2,
                canvasCenterLineY - INTER_TILE_SPACING / 2, TileType::MEDIUM};

    tiles[1] = {INTER_TILE_SPACING, canvasCenterLineY + INTER_TILE_SPACING / 2,
                canvasCenterLineX - INTER_TILE_SPACING / 2,
                screen_height_px - TILE_BOTTOM_OFFSET, TileType::MEDIUM};

    // Bottom row
    tiles[2] = {canvasCenterLineX + INTER_TILE_SPACING / 2, INTER_TILE_SPACING,
                screen_width_px - INTER_TILE_SPACING,
                canvasCenterLineY - INTER_TILE_SPACING / 2, TileType::MEDIUM};

    tiles[3] = {canvasCenterLineX + INTER_TILE_SPACING / 2,
                canvasCenterLineY + INTER_TILE_SPACING / 2,
                screen_width_px - INTER_TILE_SPACING,
                screen_height_px - TILE_BOTTOM_OFFSET, TileType::MEDIUM};

    return tiles;
}

SensorDisplayTile* _get5Tile(uint screen_width_px, uint screen_height_px) {
    SensorDisplayTile* tiles = new SensorDisplayTile[5];

    uint canvasCenterLineX = screen_width_px / 2;
    uint canvasCenterLineY = (screen_height_px - TILE_BOTTOM_OFFSET) / 2;
    uint canvasThirdLineY = (screen_height_px - TILE_BOTTOM_OFFSET) / 3;
    uint canvasTwoThirdsLineY = 2 * canvasThirdLineY;

    // Left Column
    tiles[0] = {INTER_TILE_SPACING, INTER_TILE_SPACING,
                canvasCenterLineX - INTER_TILE_SPACING / 2,
                canvasCenterLineY - INTER_TILE_SPACING / 2, TileType::MEDIUM};

    tiles[1] = {INTER_TILE_SPACING, canvasCenterLineY + INTER_TILE_SPACING / 2,
                canvasCenterLineX - INTER_TILE_SPACING / 2,
                screen_height_px - TILE_BOTTOM_OFFSET, TileType::MEDIUM};

    // Right column
    tiles[2] = {canvasCenterLineX + INTER_TILE_SPACING / 2, INTER_TILE_SPACING,
                screen_width_px - INTER_TILE_SPACING,
                canvasThirdLineY - INTER_TILE_SPACING / 2, TileType::NARROW};

    tiles[3] = {canvasCenterLineX + INTER_TILE_SPACING / 2,
                canvasThirdLineY + INTER_TILE_SPACING / 2,
                screen_width_px - INTER_TILE_SPACING,
                canvasTwoThirdsLineY - INTER_TILE_SPACING / 2,
                TileType::NARROW};

    tiles[4] = {canvasCenterLineX + INTER_TILE_SPACING / 2,
                canvasTwoThirdsLineY + INTER_TILE_SPACING / 2,
                screen_width_px - INTER_TILE_SPACING,
                screen_height_px - TILE_BOTTOM_OFFSET, TileType::NARROW};

    return tiles;
}

SensorDisplayTile* _get6Tile(uint screen_width_px, uint screen_height_px) {
    SensorDisplayTile* tiles = new SensorDisplayTile[6];

    uint canvasCenterLineY = (screen_height_px - TILE_BOTTOM_OFFSET) / 2;
    uint canvasThirdLineX = screen_width_px / 3;
    uint canvasTwoThirdsLineX = 2 * canvasThirdLineX;

    // Left column
    tiles[0] = {INTER_TILE_SPACING, INTER_TILE_SPACING,
                canvasThirdLineX - INTER_TILE_SPACING / 2,
                canvasCenterLineY - INTER_TILE_SPACING / 2, TileType::SMALL},
    tiles[1] = {INTER_TILE_SPACING, canvasCenterLineY + INTER_TILE_SPACING / 2,
                canvasThirdLineX - INTER_TILE_SPACING / 2,
                screen_height_px - TILE_BOTTOM_OFFSET, TileType::SMALL};

    // Middle
    tiles[2] = {canvasThirdLineX + INTER_TILE_SPACING / 2, INTER_TILE_SPACING,
                canvasTwoThirdsLineX - INTER_TILE_SPACING / 2,
                canvasCenterLineY - INTER_TILE_SPACING / 2, TileType::SMALL};

    tiles[3] = {canvasThirdLineX + INTER_TILE_SPACING / 2,
                canvasCenterLineY + INTER_TILE_SPACING / 2,
                canvasTwoThirdsLineX - INTER_TILE_SPACING / 2,
                screen_height_px - TILE_BOTTOM_OFFSET, TileType::SMALL};

    // Right column
    tiles[4] = {canvasTwoThirdsLineX + INTER_TILE_SPACING / 2,
                INTER_TILE_SPACING, screen_width_px - INTER_TILE_SPACING,
                canvasCenterLineY - INTER_TILE_SPACING / 2, TileType::SMALL};

    tiles[5] = {canvasTwoThirdsLineX + INTER_TILE_SPACING / 2,
                canvasCenterLineY + INTER_TILE_SPACING / 2,
                screen_width_px - INTER_TILE_SPACING,
                screen_height_px - TILE_BOTTOM_OFFSET, TileType::SMALL};

    return tiles;
}

SensorDisplayTile* _get7Tile(uint screen_width_px, uint screen_height_px) {
    SensorDisplayTile* tiles = new SensorDisplayTile[7];

    uint canvasCenterLineX = screen_width_px / 2;
    uint canvasThirdLineX = screen_width_px / 3;
    uint canvasThirdLineY = (screen_height_px - TILE_BOTTOM_OFFSET) / 3;
    uint canvasTwoThirdsLineX = 2 * canvasThirdLineX;
    uint canvasTwoThirdsLineY = 2 * canvasThirdLineY;

    // Left column
    tiles[0] = {INTER_TILE_SPACING, INTER_TILE_SPACING,
                canvasCenterLineX - INTER_TILE_SPACING / 2,
                canvasThirdLineY - INTER_TILE_SPACING / 2, TileType::NARROW};
    tiles[1] = {INTER_TILE_SPACING, canvasThirdLineY + INTER_TILE_SPACING / 2,
                canvasCenterLineX - INTER_TILE_SPACING / 2,
                canvasTwoThirdsLineY - INTER_TILE_SPACING / 2,
                TileType::NARROW};

    // Right column
    tiles[2] = {canvasCenterLineX + INTER_TILE_SPACING / 2, INTER_TILE_SPACING,
                screen_width_px - INTER_TILE_SPACING,
                canvasThirdLineY - INTER_TILE_SPACING / 2, TileType::NARROW};

    tiles[3] = {canvasCenterLineX + INTER_TILE_SPACING / 2,
                canvasThirdLineY + INTER_TILE_SPACING / 2,
                screen_width_px - INTER_TILE_SPACING,
                canvasTwoThirdsLineY - INTER_TILE_SPACING / 2,
                TileType::NARROW};

    // Bottom Row
    tiles[4] = {INTER_TILE_SPACING,
                canvasTwoThirdsLineY + INTER_TILE_SPACING / 2,
                canvasThirdLineX - INTER_TILE_SPACING / 2,
                screen_height_px - TILE_BOTTOM_OFFSET, TileType::SMALL};
    tiles[5] = {canvasThirdLineX + INTER_TILE_SPACING / 2,
                canvasTwoThirdsLineY + INTER_TILE_SPACING / 2,
                canvasTwoThirdsLineX - INTER_TILE_SPACING / 2,
                screen_height_px - TILE_BOTTOM_OFFSET, TileType::SMALL};
    tiles[6] = {canvasTwoThirdsLineX + INTER_TILE_SPACING / 2,
                canvasTwoThirdsLineY + INTER_TILE_SPACING / 2,
                screen_width_px - INTER_TILE_SPACING,
                screen_height_px - TILE_BOTTOM_OFFSET, TileType::SMALL};

    return tiles;
}

SensorDisplayTile* _get8Tile(uint screen_width_px, uint screen_height_px) {
    SensorDisplayTile* tiles = new SensorDisplayTile[8];

    uint canvasCenterLineX = screen_width_px / 2;
    uint canvasThirdLineX = screen_width_px / 3;
    uint canvasThirdLineY = (screen_height_px - TILE_BOTTOM_OFFSET) / 3;
    uint canvasTwoThirdsLineX = 2 * canvasThirdLineX;
    uint canvasTwoThirdsLineY = 2 * canvasThirdLineY;

    // Top Row
    tiles[0] = {INTER_TILE_SPACING, INTER_TILE_SPACING,
                canvasCenterLineX - INTER_TILE_SPACING / 2,
                canvasThirdLineY - INTER_TILE_SPACING / 2, TileType::NARROW};
    tiles[1] = {canvasCenterLineX + INTER_TILE_SPACING / 2, INTER_TILE_SPACING,
                screen_width_px - INTER_TILE_SPACING,
                canvasThirdLineY - INTER_TILE_SPACING / 2, TileType::NARROW};

    // Middle Row
    tiles[2] = {INTER_TILE_SPACING, canvasThirdLineY + INTER_TILE_SPACING / 2,
                canvasThirdLineX - INTER_TILE_SPACING / 2,
                canvasTwoThirdsLineY - INTER_TILE_SPACING / 2, TileType::SMALL};
    tiles[3] = {canvasThirdLineX + INTER_TILE_SPACING / 2,
                canvasThirdLineY + INTER_TILE_SPACING / 2,
                canvasTwoThirdsLineX - INTER_TILE_SPACING / 2,
                canvasTwoThirdsLineY - INTER_TILE_SPACING / 2, TileType::SMALL};
    tiles[4] = {canvasTwoThirdsLineX + INTER_TILE_SPACING / 2,
                canvasThirdLineY + INTER_TILE_SPACING / 2,
                screen_width_px - INTER_TILE_SPACING,
                canvasTwoThirdsLineY - INTER_TILE_SPACING / 2, TileType::SMALL};

    // Bottom Row
    tiles[5] = {INTER_TILE_SPACING,
                canvasTwoThirdsLineY + INTER_TILE_SPACING / 2,
                canvasThirdLineX - INTER_TILE_SPACING / 2,
                screen_height_px - TILE_BOTTOM_OFFSET, TileType::SMALL};
    tiles[6] = {canvasThirdLineX + INTER_TILE_SPACING / 2,
                canvasTwoThirdsLineY + INTER_TILE_SPACING / 2,
                canvasTwoThirdsLineX - INTER_TILE_SPACING / 2,
                screen_height_px - TILE_BOTTOM_OFFSET, TileType::SMALL};
    tiles[7] = {canvasTwoThirdsLineX + INTER_TILE_SPACING / 2,
                canvasTwoThirdsLineY + INTER_TILE_SPACING / 2,
                screen_width_px - INTER_TILE_SPACING,
                screen_height_px - TILE_BOTTOM_OFFSET, TileType::SMALL};

    return tiles;
}

SensorDisplayTile* _get1VTile(uint screen_width_px, uint screen_height_px) {
    SensorDisplayTile* tiles = new SensorDisplayTile[1];

    tiles[0] = {INTER_TILE_SPACING,
                TILE_VERTICAL_TOP_OFFSET + INTER_TILE_SPACING,
                screen_width_px - INTER_TILE_SPACING,
                screen_height_px - TILE_BOTTOM_OFFSET - INTER_TILE_SPACING,
                TileType::LARGE};
    return tiles;
}

SensorDisplayTile* _get2VTile(uint screen_width_px, uint screen_height_px) {
    SensorDisplayTile* tiles = new SensorDisplayTile[2];

    uint canvasCenterLineY =
        TILE_VERTICAL_TOP_OFFSET +
        (screen_height_px - TILE_BOTTOM_OFFSET - TILE_VERTICAL_TOP_OFFSET) / 2;

    tiles[0] = {INTER_TILE_SPACING,
                TILE_VERTICAL_TOP_OFFSET + INTER_TILE_SPACING,
                screen_width_px - INTER_TILE_SPACING,
                canvasCenterLineY - INTER_TILE_SPACING / 2, TileType::MEDIUM};

    tiles[1] = {INTER_TILE_SPACING, canvasCenterLineY + INTER_TILE_SPACING / 2,
                screen_width_px - INTER_TILE_SPACING,
                screen_height_px - TILE_BOTTOM_OFFSET - INTER_TILE_SPACING,
                TileType::MEDIUM};

    return tiles;
}

SensorDisplayTile* _get3VTile(uint screen_width_px, uint screen_height_px) {
    SensorDisplayTile* tiles = new SensorDisplayTile[3];

    uint canvasThirdLineY =
        TILE_VERTICAL_TOP_OFFSET +
        (screen_height_px - TILE_BOTTOM_OFFSET - TILE_VERTICAL_TOP_OFFSET) / 3;
    uint canvasTwoThirdLineY =
        TILE_VERTICAL_TOP_OFFSET +
        2 * (screen_height_px - TILE_BOTTOM_OFFSET - TILE_VERTICAL_TOP_OFFSET) /
            3;

    tiles[0] = {INTER_TILE_SPACING,
                TILE_VERTICAL_TOP_OFFSET + INTER_TILE_SPACING,
                screen_width_px - INTER_TILE_SPACING,
                canvasThirdLineY - INTER_TILE_SPACING / 2, TileType::MEDIUM};

    tiles[1] = {INTER_TILE_SPACING, canvasThirdLineY + INTER_TILE_SPACING / 2,
                screen_width_px - INTER_TILE_SPACING,
                canvasTwoThirdLineY - INTER_TILE_SPACING / 2, TileType::MEDIUM};

    tiles[2] = {INTER_TILE_SPACING,
                canvasTwoThirdLineY + INTER_TILE_SPACING / 2,
                screen_width_px - INTER_TILE_SPACING,
                screen_height_px - TILE_BOTTOM_OFFSET - INTER_TILE_SPACING,
                TileType::MEDIUM};
    return tiles;
}

SensorDisplayTile* _get4VTile(uint screen_width_px, uint screen_height_px) {
    SensorDisplayTile* tiles = new SensorDisplayTile[4];

    uint canvasCenterLineY =
        TILE_VERTICAL_TOP_OFFSET +
        (screen_height_px - TILE_BOTTOM_OFFSET - TILE_VERTICAL_TOP_OFFSET) / 2;
    uint canvasCenterLineX = screen_width_px / 2;

    tiles[0] = {INTER_TILE_SPACING,
                TILE_VERTICAL_TOP_OFFSET + INTER_TILE_SPACING,
                canvasCenterLineX - INTER_TILE_SPACING / 2,
                canvasCenterLineY - INTER_TILE_SPACING / 2, TileType::SMALL};

    tiles[1] = {INTER_TILE_SPACING, canvasCenterLineY + INTER_TILE_SPACING / 2,
                canvasCenterLineX - INTER_TILE_SPACING / 2,
                screen_height_px - TILE_BOTTOM_OFFSET - INTER_TILE_SPACING,
                TileType::SMALL};

    tiles[2] = {canvasCenterLineX + INTER_TILE_SPACING / 2,
                TILE_VERTICAL_TOP_OFFSET + INTER_TILE_SPACING,
                screen_width_px - INTER_TILE_SPACING,
                canvasCenterLineY - INTER_TILE_SPACING / 2, TileType::SMALL};

    tiles[3] = {canvasCenterLineX + INTER_TILE_SPACING / 2,
                canvasCenterLineY + INTER_TILE_SPACING / 2,
                screen_width_px - INTER_TILE_SPACING,
                screen_height_px - TILE_BOTTOM_OFFSET - INTER_TILE_SPACING,
                TileType::SMALL};
    return tiles;
}
SensorDisplayTile* _get5VTile(uint screen_width_px, uint screen_height_px) {
    SensorDisplayTile* tiles = new SensorDisplayTile[5];

    uint canvasThirdLineY =
        TILE_VERTICAL_TOP_OFFSET +
        (screen_height_px - TILE_BOTTOM_OFFSET - TILE_VERTICAL_TOP_OFFSET) / 3;
    uint canvasTwoThirdLineY =
        TILE_VERTICAL_TOP_OFFSET +
        2 * (screen_height_px - TILE_BOTTOM_OFFSET - TILE_VERTICAL_TOP_OFFSET) /
            3;
    uint canvasCenterLineX = screen_width_px / 2;

    tiles[0] = {INTER_TILE_SPACING,
                TILE_VERTICAL_TOP_OFFSET + INTER_TILE_SPACING,
                screen_width_px - INTER_TILE_SPACING,
                canvasThirdLineY - INTER_TILE_SPACING / 2, TileType::MEDIUM};

    tiles[1] = {INTER_TILE_SPACING, canvasThirdLineY + INTER_TILE_SPACING / 2,
                canvasCenterLineX - INTER_TILE_SPACING / 2,
                canvasTwoThirdLineY - INTER_TILE_SPACING / 2, TileType::SMALL};

    tiles[2] = {INTER_TILE_SPACING,
                canvasTwoThirdLineY + INTER_TILE_SPACING / 2,
                canvasCenterLineX - INTER_TILE_SPACING / 2,
                screen_height_px - TILE_BOTTOM_OFFSET - INTER_TILE_SPACING,
                TileType::SMALL};

    tiles[3] = {canvasCenterLineX + INTER_TILE_SPACING / 2,
                canvasThirdLineY + INTER_TILE_SPACING / 2,
                screen_width_px - INTER_TILE_SPACING,
                canvasTwoThirdLineY - INTER_TILE_SPACING / 2, TileType::SMALL};

    tiles[4] = {canvasCenterLineX + INTER_TILE_SPACING / 2,
                canvasTwoThirdLineY + INTER_TILE_SPACING / 2,
                screen_width_px - INTER_TILE_SPACING,
                screen_height_px - TILE_BOTTOM_OFFSET - INTER_TILE_SPACING,
                TileType::SMALL};
    return tiles;
}
SensorDisplayTile* _get6VTile(uint screen_width_px, uint screen_height_px) {
    SensorDisplayTile* tiles = new SensorDisplayTile[6];

    uint canvasThirdLineY =
        TILE_VERTICAL_TOP_OFFSET +
        (screen_height_px - TILE_BOTTOM_OFFSET - TILE_VERTICAL_TOP_OFFSET) / 3;
    uint canvasTwoThirdLineY =
        TILE_VERTICAL_TOP_OFFSET +
        2 * (screen_height_px - TILE_BOTTOM_OFFSET - TILE_VERTICAL_TOP_OFFSET) /
            3;
    uint canvasCenterLineX = screen_width_px / 2;

    tiles[0] = {INTER_TILE_SPACING,
                TILE_VERTICAL_TOP_OFFSET + INTER_TILE_SPACING,
                canvasCenterLineX - INTER_TILE_SPACING / 2,
                canvasThirdLineY - INTER_TILE_SPACING / 2, TileType::SMALL};

    tiles[1] = {canvasCenterLineX + INTER_TILE_SPACING / 2,
                TILE_VERTICAL_TOP_OFFSET + INTER_TILE_SPACING,
                screen_width_px - INTER_TILE_SPACING,
                canvasThirdLineY - INTER_TILE_SPACING / 2, TileType::SMALL};

    tiles[2] = {INTER_TILE_SPACING, canvasThirdLineY + INTER_TILE_SPACING / 2,
                canvasCenterLineX - INTER_TILE_SPACING / 2,
                canvasTwoThirdLineY - INTER_TILE_SPACING / 2, TileType::SMALL};

    tiles[3] = {INTER_TILE_SPACING,
                canvasTwoThirdLineY + INTER_TILE_SPACING / 2,
                canvasCenterLineX - INTER_TILE_SPACING / 2,
                screen_height_px - TILE_BOTTOM_OFFSET - INTER_TILE_SPACING,
                TileType::SMALL};

    tiles[4] = {canvasCenterLineX + INTER_TILE_SPACING / 2,
                canvasThirdLineY + INTER_TILE_SPACING / 2,
                screen_width_px - INTER_TILE_SPACING,
                canvasTwoThirdLineY - INTER_TILE_SPACING / 2, TileType::SMALL};

    tiles[5] = {canvasCenterLineX + INTER_TILE_SPACING / 2,
                canvasTwoThirdLineY + INTER_TILE_SPACING / 2,
                screen_width_px - INTER_TILE_SPACING,
                screen_height_px - TILE_BOTTOM_OFFSET - INTER_TILE_SPACING,
                TileType::SMALL};
    return tiles;
}

}  // namespace sensirion::upt::display
