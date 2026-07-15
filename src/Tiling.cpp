#include "Tiling.h"

namespace sensirion::upt::display {

constexpr auto TAG = "VIZ";
constexpr uint16_t kMinTileWidthPx = 46;
constexpr uint16_t kMinTileHeightPx = 70;

// Returns the TileType based on the tile's pixel width.
static TileType getTileType(const uint16_t tileWidthPx,
                            const uint16_t tileHeightPx) {
    if (tileWidthPx >= 110 && tileHeightPx >= 110)
        return TileType::LARGE;
    if (tileWidthPx >= 95 && tileHeightPx >= 95)
        return TileType::MEDIUM;
    if (tileWidthPx >= 80 && tileHeightPx >= 80)
        return TileType::NARROW;
    return TileType::SMALL;
}

static uint8_t getMaxNumTiles(const uint16_t screenWidthPx,
                              const uint16_t screenHeightPx) {
    const bool isLandscape = screenWidthPx >= screenHeightPx;

    const uint16_t offset =
        isLandscape ? TILE_OFFSET : 2 * TILE_OFFSET - TILE_MARGIN;

    uint16_t maxColumns = 0;
    // at least one column fits on the screen
    if (screenWidthPx >= kMinTileWidthPx + 2 * TILE_MARGIN) {
        maxColumns =
            (screenWidthPx - TILE_MARGIN) / (kMinTileWidthPx + TILE_MARGIN);
    }

    uint16_t maxRows = 0;
    // at least on row fits on the screen
    if (screenHeightPx >= (offset + kMinTileHeightPx + TILE_MARGIN)) {
        maxRows = (screenHeightPx - offset) / (kMinTileHeightPx + TILE_MARGIN);
    }

    if (maxColumns == 0u || maxRows == 0u) {
        return 0;
    }

    // clamp max tiles to 255 so it fits in a uint8_t
    auto maxTiles = maxColumns * maxRows;
    if (maxTiles > 255) {
        maxTiles = 255;
    }

    return static_cast<uint8_t>(maxTiles);
}

static uint8_t calculateOptimalTileRows(const uint16_t numTiles,
                                        const uint16_t canvasW,
                                        const uint16_t canvasH) {
    // Find the number of rows R that maximizes the average tile area,
    // subject to each tile meeting the minimum dimension constraints.
    uint8_t bestRows = 1;

    // Special case 3 tiles: always split into 1 tile on top,
    //                       2 tiles on the bottom.
    // Special case 4 tiles: always split into 2 tiles on top,
    //                       2 tiles on the bottom.
    if (numTiles == 3 || numTiles == 4) {
        bestRows = 2;
    } else {
        uint32_t bestAvgArea = 0;
        for (uint16_t r = 1; r <= numTiles; r++) {
            const uint8_t b = numTiles / r;
            if (b == 0)
                break;
            const uint8_t e = numTiles % r;

            // Approximate tile height (ignoring subpixel rounding)
            if (canvasH <= static_cast<uint16_t>((r - 1) * TILE_MARGIN))
                continue;
            const uint32_t tileH = (canvasH - (r - 1) * TILE_MARGIN) / r;
            if (tileH < kMinTileHeightPx)
                continue;

            // Tile widths for the two column counts
            const uint32_t tileW_base =
                (canvasW > static_cast<uint16_t>((b - 1) * TILE_MARGIN))
                    ? (canvasW - (b - 1) * TILE_MARGIN) / b
                    : 0;
            const uint32_t tileW_extra =
                (e > 0 && canvasW > static_cast<uint16_t>(b * TILE_MARGIN))
                    ? (canvasW - b * TILE_MARGIN) / (b + 1)
                    : tileW_base;

            if (tileW_base < kMinTileWidthPx || tileW_extra < kMinTileWidthPx)
                continue;

            const uint32_t totalArea = (r - e) * b * tileH * tileW_base +
                                       e * (b + 1) * tileH * tileW_extra;
            const uint32_t avgArea = totalArea / numTiles;

            if (avgArea > bestAvgArea) {
                bestAvgArea = avgArea;
                bestRows = r;
            }
        }
    }
    return bestRows;
}

SensorDisplayTile* getNTiles(const uint16_t numTiles,
                             const uint16_t screenWidthPx,
                             const uint16_t screenHeightPx) {
    const uint16_t maxTiles = getMaxNumTiles(screenWidthPx, screenHeightPx);
    if (numTiles == 0 || numTiles > maxTiles) {
        ESP_LOGE(TAG,
                 "Requested too many tiles for the current configuration "
                 "%d tiles for screen %d x %d",
                 maxTiles, screenWidthPx, screenHeightPx);
        return nullptr;
    }

    const bool isLandscape = screenWidthPx >= screenHeightPx;

    // Offset area is at the bottom in landscape mode, at the top and bottom
    // in portrait mode.
    constexpr int16_t canvasLeft = TILE_MARGIN;
    const int16_t canvasTop = isLandscape ? static_cast<int16_t>(TILE_MARGIN)
                                          : static_cast<int16_t>(TILE_OFFSET);
    const auto canvasRight = static_cast<int16_t>(screenWidthPx - TILE_MARGIN);
    const auto canvasBottom =
        static_cast<int16_t>(screenHeightPx - TILE_OFFSET);

    const auto canvasW = static_cast<uint16_t>(canvasRight - canvasLeft);
    const auto canvasH = static_cast<uint16_t>(canvasBottom - canvasTop);

    auto* tiles = new SensorDisplayTile[numTiles];

    if (numTiles == 1) {
        tiles[0] = {{canvasLeft, canvasTop}, canvasW, canvasH, TileType::LARGE};
        return tiles;
    }

    const uint8_t bestRows =
        calculateOptimalTileRows(numTiles, canvasW, canvasH);

    const uint8_t base = numTiles / bestRows;
    const uint8_t extra = numTiles % bestRows;

    uint8_t tileIdx = 0;
    for (uint8_t row = 0; row < bestRows; row++) {
        const uint8_t colsInRow = (row < bestRows - extra) ? base : base + 1;

        // Row vertical span: divide canvas at fractional lines to avoid
        // accumulated rounding errors across rows.
        const int16_t rowTop =
            (row == 0) ? canvasTop
                       : static_cast<int16_t>(canvasTop +
                                              static_cast<int32_t>(canvasH) *
                                                  row / bestRows +
                                              TILE_MARGIN / 2);
        const int16_t rowBottom =
            (row == bestRows - 1)
                ? canvasBottom
                : static_cast<int16_t>(canvasTop +
                                       static_cast<int32_t>(canvasH) *
                                           (row + 1) / bestRows -
                                       TILE_MARGIN / 2);

        for (uint8_t col = 0; col < colsInRow; col++) {
            // Column horizontal span within this row.
            const int16_t colLeft =
                (col == 0)
                    ? canvasLeft
                    : static_cast<int16_t>(canvasLeft +
                                           static_cast<int32_t>(canvasW) * col /
                                               colsInRow +
                                           TILE_MARGIN / 2);
            const int16_t colRight =
                (col == colsInRow - 1)
                    ? canvasRight
                    : static_cast<int16_t>(canvasLeft +
                                           static_cast<int32_t>(canvasW) *
                                               (col + 1) / colsInRow -
                                           TILE_MARGIN / 2);

            const auto tileWidth = static_cast<uint16_t>(colRight - colLeft);
            const auto tileHeight = static_cast<uint16_t>(rowBottom - rowTop);
            tiles[tileIdx++] = {{colLeft, rowTop},
                                tileWidth,
                                tileHeight,
                                getTileType(tileWidth, tileHeight)};
        }
    }

    return tiles;
}

}  // namespace sensirion::upt::display
