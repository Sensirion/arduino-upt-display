#include "UptDisplay.h"
#include "SignalTypeExtended.h"
#include "TftDisplay.h"
#include <SensirionColors.h>
#include <Tiling.h>
#include <sstream>

namespace sensirion::upt::display {

#define ROUNDED_CORNER_RADIUS 10
#define SCREEN_FRAME_MARGIN 2
#define MEASUREMENT_VALUE_UNIT_SPACING 5
#define TILE_TITLE_OFFSET 20

// Dev grid overlay can be enabled by compiling with the flag
// UPTDISPLAY_SHOW_GRID
static void drawDevOverlay();

static void drawBackground();
static void drawVScreenTopTitle(const SensorDisplayValues& sensorData);
static void drawVScreenLegend(const SensorDisplayValues& sensorData);
static void drawHScreenLegend(const SensorDisplayValues& sensorData);
static void drawTile(const SensorDisplayTile& tile,
                     const core::Measurement& measurement);
static void drawTileValue(const SensorDisplayTile& tile,
                          const core::Measurement& measurement);
static void eraseTileValue(const SensorDisplayTile& tile);

/* Buffer a signal as a string */
static std::string bufferValueAsString(const core::Measurement& measurement);

/* Get color with which a signal should be displayed */
static uint32_t colorOf(const core::Measurement& measurement);

static TftDisplay _defaultDisplay;
static IDisplay* _display = nullptr;

static int16_t drawXPos;
static int16_t drawYPos;

// ReSharper disable once CppDeclaratorNeverUsed
static auto TAG = "VIZ";

void init(const Orientation orientation) {
    init(_defaultDisplay, orientation);
}

void init(IDisplay& display, const Orientation orientation) {
    _display = &display;
    _display->init(orientation);
    drawBackground();
}

void showTextScreen(const char* text) {
    drawBackground();

#ifdef UPTDISPLAY_SHOW_GRID
    drawDevOverlay();
#endif /* UPTDISPLAY_SHOW_GRID */

    const auto cursorX =
        static_cast<int16_t>(_display->getWidth() / 2 -
                             _display->getTextWidth(text, Font::MEDIUM) / 2);
    const auto cursorY = static_cast<int16_t>(
        _display->getHeight() / 2 - _display->getFontHeight(Font::MEDIUM) / 2);

    _display->drawText(text, cursorX, cursorY, Font::MEDIUM,
                       UPT_DISPLAY_FONT_PRIMARY_COLOR,
                       UPT_DISPLAY_BACKGROUND_COLOR);
}

void showInformationScreen(
    const std::vector<std::pair<std::string, std::string>>& information,
    const byte* image, const int16_t imageHeight, const int16_t imageWidth) {
    constexpr char title[12] = "INFORMATION";
    uint16_t info_area_width_px;

    // Wipe screen
    drawBackground();

    // Define image location
    if (_display->getRotation() == Orientation::portrait) {
        // Vertical
        // Image is below the information
        drawXPos = 10;
        drawYPos = static_cast<int16_t>(_display->getHeight() / 2);
        info_area_width_px = _display->getWidth();
    } else {
        // Horizontal:
        // Image is next to the information
        drawXPos = static_cast<int16_t>(_display->getWidth() / 2);
        drawYPos = 10;
        info_area_width_px = static_cast<int16_t>(_display->getWidth() / 2);
    }

    // Print image
    _display->drawXBitmap(drawXPos, drawYPos, image, imageHeight, imageWidth,
                          UPT_DISPLAY_FONT_PRIMARY_COLOR);

    const auto x_value = static_cast<int16_t>(info_area_width_px / 2 + 5);
    int16_t y_line = 10;

    const int f_height = _display->getFontHeight(Font::MEDIUM);
    const int title_width_px = _display->getTextWidth(title, Font::MEDIUM);

    const auto xCursor =
        static_cast<int16_t>(info_area_width_px / 2 - title_width_px / 2);
    _display->drawText(title, xCursor, y_line, Font::MEDIUM,
                       UPT_DISPLAY_FONT_PRIMARY_COLOR,
                       UPT_DISPLAY_BACKGROUND_COLOR);

    y_line += 25;

    for (const auto& info : information) {
        constexpr int x_key = 10;
        _display->drawText(info.first.c_str(), x_key, y_line, Font::MEDIUM,
                           UPT_DISPLAY_FONT_PRIMARY_COLOR,
                           UPT_DISPLAY_BACKGROUND_COLOR);

        _display->drawText(info.second.c_str(), x_value, y_line, Font::MEDIUM,
                           UPT_DISPLAY_FONT_PRIMARY_COLOR,
                           UPT_DISPLAY_BACKGROUND_COLOR);

        y_line = static_cast<int16_t>(y_line + f_height + 6);
    }
}

void showSensorData(const SensorDisplayValues& data) {
    uint n_value = data.measurements.size();

    // Wipe screen
    drawBackground();

    if (n_value == 0 || n_value > 8) {
        showTextScreen("Sensor has no configured signals.");
        return;
    }
    if (_display->getRotation() == Orientation::portrait) {
        // Vertical screen has a top title
        drawVScreenTopTitle(data);
        drawVScreenLegend(data);
    } else {
        drawHScreenLegend(data);
    }

    const SensorDisplayTile* tiles =
        getNTiles(n_value, _display->getWidth(), _display->getHeight());

    if (tiles == nullptr)
        return;

    for (int i = 0; i < n_value; i++) {
        drawTile(tiles[i], data.measurements[i]);
        drawTileValue(tiles[i], data.measurements[i]);
    }

    delete[] tiles;

#ifdef UPTDISPLAY_SHOW_GRID
    drawDevOverlay();
#endif /* UPTDISPLAY_SHOW_GRID */
}

void refreshSensorData(const SensorDisplayValues& data) {
    uint n_value = data.measurements.size();

    if (n_value == 0 || n_value > 8) {
        showTextScreen("Sensor has no configured signals.");
        return;
    }

    if (_display->getRotation() == Orientation::portrait) {
        drawVScreenLegend(data);
    } else {
        drawHScreenLegend(data);
    }

    const SensorDisplayTile* tiles =
        getNTiles(n_value, _display->getWidth(), _display->getHeight());

    if (tiles == nullptr)
        return;

    for (int i = 0; i < n_value; i++) {
        eraseTileValue(tiles[i]);
        drawTileValue(tiles[i], data.measurements[i]);
    }

    delete[] tiles;

#ifdef UPTDISPLAY_SHOW_GRID
    drawDevOverlay();
#endif /* UPTDISPLAY_SHOW_GRID */
}

void drawDevOverlay() {
    constexpr int sep = 50;

    // Draw red grid lines
    for (uint16_t x = sep; x < _display->getWidth(); x += sep) {
        _display->drawVLine(x, 0, _display->getHeight(),
                                UPT_DISPLAY_RED_COLOR);
    }
    for (uint16_t y = sep; y < _display->getHeight(); y += sep) {
        _display->drawHLine(0, y, _display->getWidth(),
                                UPT_DISPLAY_RED_COLOR);
    }

    // Draw coordinate info
    for (int x = sep; x < _display->getWidth(); x += sep) {
        char lbl[4];
        sprintf(lbl, "x%i", x);
        const auto cursorX = static_cast<int16_t>(
            x - _display->getTextWidth(lbl, Font::NORMAL) / 2 + 1);
        _display->drawText(lbl, cursorX, 0, Font::NORMAL, UPT_DISPLAY_RED_COLOR,
                           UPT_DISPLAY_BACKGROUND_COLOR);
    }
    for (uint16_t y = sep; y < _display->getHeight(); y += sep) {
        char lbl[4];
        sprintf(lbl, "y%i", y);
        const auto cursorY = static_cast<int16_t>(
            y - _display->getFontHeight(Font::NORMAL) / 2 + 1);
        _display->drawText(lbl, 0, cursorY, Font::NORMAL, UPT_DISPLAY_RED_COLOR,
                           UPT_DISPLAY_BACKGROUND_COLOR);
    }
}

void drawBackground() {
    _display->fillScreen(UPT_DISPLAY_BACKGROUND_COLOR);
}

void drawVScreenTopTitle(const SensorDisplayValues& sensorData) {
    // Print sensor name
    const auto cursorX = static_cast<int16_t>(
        _display->getWidth() / 2 -
        _display->getTextWidth(sensorData.sensorName.c_str(), Font::MEDIUM) /
            2);
    _display->drawText(sensorData.sensorName.c_str(), cursorX,
                       TILE_OFFSET - _display->getFontHeight(Font::MEDIUM),
                       Font::MEDIUM, UPT_DISPLAY_FONT_PRIMARY_COLOR,
                       UPT_DISPLAY_BACKGROUND_COLOR);
}

void drawVScreenLegend(const SensorDisplayValues& sensorData) {
    int16_t cursorX = TILE_MARGIN;
    const auto cursorY = static_cast<int16_t>(
        _display->getHeight() - _display->getFontHeight(Font::MEDIUM) -
        SCREEN_FRAME_MARGIN);

    // Print measurement time
    _display->drawText(sensorData.timeInfoStr.c_str(), cursorX, cursorY,
                       Font::MEDIUM, UPT_DISPLAY_FONT_PRIMARY_COLOR,
                       UPT_DISPLAY_BACKGROUND_COLOR);

    // Print sensor rank
    char rank[8];
    sprintf(rank, " %i/%i", sensorData.sensorRank,
            sensorData.numTrackedSensors);
    cursorX = static_cast<int16_t>(_display->getWidth() - TILE_MARGIN -
                                   _display->getTextWidth(rank, Font::MEDIUM));
    _display->drawText(rank, cursorX, cursorY, Font::MEDIUM,
                       UPT_DISPLAY_FONT_PRIMARY_COLOR,
                       UPT_DISPLAY_BACKGROUND_COLOR);
}

void drawHScreenLegend(const SensorDisplayValues& sensorData) {
    // Print sensor name
    auto cursorX = static_cast<int16_t>(SCREEN_FRAME_MARGIN + 5);
    auto cursorY = static_cast<int16_t>(_display->getHeight() -
                                        _display->getFontHeight(Font::MEDIUM) -
                                        SCREEN_FRAME_MARGIN);

    char lbl[32];
    sprintf(lbl, "%s (%i/%i)", sensorData.sensorName.c_str(),
            sensorData.sensorRank, sensorData.numTrackedSensors);

    _display->drawText(lbl, cursorX, cursorY, Font::MEDIUM,
                       UPT_DISPLAY_FONT_PRIMARY_COLOR,
                       UPT_DISPLAY_BACKGROUND_COLOR);

    const uint16_t sensorLabelWidth = _display->getTextWidth(lbl, Font::MEDIUM);

    // Print measurement time
    cursorX = static_cast<int16_t>(cursorX + sensorLabelWidth + 5);
    cursorY = static_cast<int16_t>(_display->getHeight() -
                                   _display->getFontHeight(Font::SMALL) -
                                   SCREEN_FRAME_MARGIN);
    _display->drawText(sensorData.timeInfoStr.c_str(), cursorX, cursorY,
                       Font::SMALL, UPT_DISPLAY_FONT_PRIMARY_COLOR,
                       UPT_DISPLAY_BACKGROUND_COLOR);
}

void drawTile(const SensorDisplayTile& tile,
              const core::Measurement& measurement) {
    // Draw Tile
    _display->fillRoundRect(tile.topLeft.x, tile.topLeft.y, tile.width,
                            tile.height, ROUNDED_CORNER_RADIUS,
                            UPT_DISPLAY_TILE_PRIMARY_COLOR);

    // Print signal description
    const auto cursorX = static_cast<int16_t>(tile.topLeft.x + 10);
    const auto cursorY = static_cast<int16_t>(tile.topLeft.y + 5);

    switch (tile.type) {
        case TileType::SMALL:
            if (_display->getRotation() == Orientation::landscape) {
                _display->drawText(
                    shortSignalDescription(measurement.signalType).c_str(),
                    cursorX, cursorY, Font::SMALL,
                    UPT_DISPLAY_FONT_PRIMARY_COLOR,
                    UPT_DISPLAY_TILE_PRIMARY_COLOR);
            } else {
                _display->drawText(
                    shortSignalDescription(measurement.signalType).c_str(),
                    cursorX, cursorY, Font::MEDIUM,
                    UPT_DISPLAY_FONT_PRIMARY_COLOR,
                    UPT_DISPLAY_TILE_PRIMARY_COLOR);
            }
            break;
        case TileType::NARROW:
        case TileType::MEDIUM:
            _display->drawText(
                medSignalDescription(measurement.signalType).c_str(), cursorX,
                cursorY, Font::MEDIUM, UPT_DISPLAY_FONT_PRIMARY_COLOR,
                UPT_DISPLAY_TILE_PRIMARY_COLOR);
            break;
        case TileType::LARGE:
            _display->drawText(
                longSignalDescription(measurement.signalType).c_str(), cursorX,
                cursorY, Font::MEDIUM, UPT_DISPLAY_FONT_PRIMARY_COLOR,
                UPT_DISPLAY_TILE_PRIMARY_COLOR);
            break;
        default:
            break;
    }
}

void eraseTileValue(const SensorDisplayTile& tile) {
    const auto rectX = tile.topLeft.x;
    const auto rectY = static_cast<int16_t>(tile.topLeft.y + TILE_TITLE_OFFSET);
    const auto rectW = tile.width;
    const auto rectH = static_cast<int16_t>(tile.getBottomRight().y - rectY);

    _display->fillRoundRect(rectX, rectY, rectW, rectH, ROUNDED_CORNER_RADIUS,
                            UPT_DISPLAY_TILE_PRIMARY_COLOR);
}

void drawTileValue(const SensorDisplayTile& tile,
                   const core::Measurement& measurement) {
    const auto val = bufferValueAsString(measurement);
    uint xShiftValue = 0;
    uint yShiftValue = 0;
    Font valueFont;

    switch (tile.type) {
        case TileType::SMALL:
            // Shift value if horizontal tile is too small to avoid overflow
            if (_display->getRotation() == Orientation::landscape) {
                xShiftValue = 15;
                yShiftValue = 2;
            }
        case TileType::NARROW:
            valueFont = Font::LARGE;
            break;
        case TileType::MEDIUM:
        case TileType::LARGE:
            valueFont = Font::XLARGE;
            break;
        default:
            valueFont = Font::MEDIUM;
            break;
    }

    const uint16_t valWidth = _display->getTextWidth(val.c_str(), valueFont);
    const uint16_t valFontHeight = _display->getFontHeight(valueFont);

    // Offset height because of title
    const auto [cx, cy] = tile.getCenter();
    const auto cursorX = static_cast<int16_t>(cx - valWidth / 2 - xShiftValue);
    const auto cursorY = static_cast<int16_t>(cy + TILE_TITLE_OFFSET -
                                              valFontHeight + yShiftValue);

    _display->drawText(val.c_str(), cursorX, cursorY, valueFont,
                       colorOf(measurement), UPT_DISPLAY_TILE_PRIMARY_COLOR);

    const std::string unit = getGraphicSignalUnit(measurement.signalType);
    Font unitFont;

    switch (tile.type) {
        case TileType::SMALL:
        case TileType::NARROW:
            unitFont = Font::SMALL;
            break;
        case TileType::MEDIUM:
        case TileType::LARGE:
        default:
            unitFont = Font::MEDIUM;
            break;
    }

    int16_t unitXPos, unitYPos;
    if (_display->getRotation() == Orientation::landscape) {
        unitXPos = static_cast<int16_t>(tile.getCenter().x +
                                        MEASUREMENT_VALUE_UNIT_SPACING +
                                        valWidth / 2 - xShiftValue);
        // Note: here we shift by 1/4 of font height because we need to ignore
        // the descendant part of the font, and we estimate it at 1/4th of the
        // height.
        unitYPos =
            static_cast<int16_t>(cursorY + 3 * valFontHeight / 4 -
                                 3 * _display->getFontHeight(unitFont) / 4);
    } else {

        const auto [brx, bry] = tile.getBottomRight();
        unitXPos = static_cast<int16_t>(
            brx - ROUNDED_CORNER_RADIUS -
            _display->getTextWidth(unit.c_str(), unitFont));
        unitYPos =
            static_cast<int16_t>(bry - ROUNDED_CORNER_RADIUS -
                                 3 * _display->getFontHeight(unitFont) / 4);
    }

    _display->drawText(unit.c_str(), unitXPos, unitYPos, unitFont,
                       UPT_DISPLAY_FONT_PRIMARY_COLOR,
                       UPT_DISPLAY_TILE_PRIMARY_COLOR);
}

std::string bufferValueAsString(const core::Measurement& measurement) {
    std::stringstream ss{};
    ss << std::fixed;  // use fixed notation for numbers.
    const core::SignalType st = measurement.signalType;
    if (st == core::SignalType::TEMPERATURE_DEGREES_CELSIUS ||
        st == core::SignalType::TEMPERATURE_DEGREES_FARENHEIT ||
        st == core::SignalType::RELATIVE_HUMIDITY_PERCENTAGE ||
        st == core::SignalType::VELOCITY_METERS_PER_SECOND ||
        st == core::SignalType::GAS_CONCENTRATION_VOLUME_PERCENTAGE ||
        st == core::SignalType::H2_CONCENTRATION_VOLUME_PERCENTAGE) {
        ss.precision(1);
    } else {
        ss.precision(0);
    }

    ss << measurement.dataPoint.value;
    std::string val = ss.str();
    const auto value_str_size = static_cast<int16_t>(3 - val.size());
    if (value_str_size > 0) {
        // A workaround because less than 3 char is randomly not being
        // displayed. Prepending empty strings to make it a 3-character string.
        // This is preferred over a regular space which may be stripped or
        // ignored by the rendering engine.
        return std::string(value_str_size, ' ') + val;
    }

    return val;
}

uint32_t colorOf(const core::Measurement& measurement) {
    switch (measurement.signalType) {
        case core::SignalType::TEMPERATURE_DEGREES_CELSIUS: {
            if (measurement.dataPoint.value < 10) {
                return UPT_DISPLAY_BLUE_COLOR;
            }
            if (measurement.dataPoint.value > 30) {
                return UPT_DISPLAY_RED_COLOR;
            }
            return UPT_DISPLAY_GREEN_COLOR;
        }
        case core::SignalType::TEMPERATURE_DEGREES_FARENHEIT: {
            if (measurement.dataPoint.value < 50) {
                return UPT_DISPLAY_BLUE_COLOR;
            }
            if (measurement.dataPoint.value > 86) {
                return UPT_DISPLAY_RED_COLOR;
            }
            return UPT_DISPLAY_GREEN_COLOR;
        }
        case core::SignalType::RELATIVE_HUMIDITY_PERCENTAGE:
            return UPT_DISPLAY_BLUE_COLOR;
        case core::SignalType::CO2_PARTS_PER_MILLION: {
            if (measurement.dataPoint.value < 650) {
                return UPT_DISPLAY_GREEN_COLOR;
            }
            if (measurement.dataPoint.value < 1200) {
                return UPT_DISPLAY_YELLOW_COLOR;
            }
            return UPT_DISPLAY_RED_COLOR;
        }
        case core::SignalType::  // NOLINT(*-branch-clone)
            HCHO_PARTS_PER_BILLION:
            return UPT_DISPLAY_RED_COLOR;
        case core::SignalType::PM1P0_MICRO_GRAMM_PER_CUBIC_METER:
        case core::SignalType::PM2P5_MICRO_GRAMM_PER_CUBIC_METER:
        case core::SignalType::PM4P0_MICRO_GRAMM_PER_CUBIC_METER:
        case core::SignalType::PM10P0_MICRO_GRAMM_PER_CUBIC_METER:
            return UPT_DISPLAY_RED_COLOR;
        case core::SignalType::VELOCITY_METERS_PER_SECOND:
            return UPT_DISPLAY_GRAY_COLOR;
        case core::SignalType::RAW_VOC_INDEX:
        case core::SignalType::RAW_NOX_INDEX:
        case core::SignalType::VOC_INDEX: {
            if (measurement.dataPoint.value < 85) {
                return UPT_DISPLAY_BLUE_COLOR;
            }
            if (measurement.dataPoint.value < 105) {
                return UPT_DISPLAY_GREEN_COLOR;
            }
            if (measurement.dataPoint.value < 110) {
                return UPT_DISPLAY_YELLOW_COLOR;
            }
            return UPT_DISPLAY_RED_COLOR;
        }
        case core::SignalType::NOX_INDEX: {
            if (measurement.dataPoint.value <= 1) {
                return UPT_DISPLAY_GREEN_COLOR;
            }
            if (measurement.dataPoint.value < 5) {
                return UPT_DISPLAY_YELLOW_COLOR;
            }
            return UPT_DISPLAY_RED_COLOR;
        }
        case core::SignalType::GAS_CONCENTRATION_VOLUME_PERCENTAGE:
        case core::SignalType::H2_CONCENTRATION_VOLUME_PERCENTAGE:
        case core::SignalType::PRESSURE_MBAR:
            return UPT_DISPLAY_GREEN_COLOR;
        default:
            return 0;
    }
}

}  // namespace sensirion::upt::display
