#include "UptDisplay.h"
#include "SignalTypeExtended.h"
#include <SensirionColors.h>
#include <Tiling.h>
#include <fonts/DefaultFont.h>
#include <sstream>

namespace sensirion::upt::display {

#define ROUNDED_CORNER_RADIUS 10
#define SCREEN_FRAME_MARGIN 2
#define MEASUREMENT_VALUE_UNIT_SPACING 5
#define TILE_TITLE_OFFSET 20

// Dev grid overlay can be enabled by compiling with the flag
// UPTDISPLAY_SHOW_GRID
void drawDevOverlay();

void drawBackground();
void drawVScreenTopTitle(const SensorDisplayValues& sensorData);
void drawVScreenLegend(const SensorDisplayValues& sensorData);
void drawHScreenLegend(const SensorDisplayValues& sensorData);
void drawTile(const SensorDisplayTile& tile,
              const core::Measurement& measurement);
void drawTileValue(const SensorDisplayTile& tile,
                   const core::Measurement& measurement);
void eraseTileValue(const SensorDisplayTile& tile);

/* Buffer a signal as a string */
std::string bufferValueAsString(const core::Measurement& measurement);

/* Get color with which a signal should be displayed */
uint32_t colorOf(const core::Measurement& measurement);

TFT_eSPI tft;
auto spr = TFT_eSprite(&tft);

static int16_t drawXPos;
static int16_t drawYPos;

// ReSharper disable once CppDeclaratorNeverUsed
static auto TAG = "VIZ";

void init(const Orientation orientation) {
    tft.init();
    tft.setTextWrap(false);
    tft.setRotation(orientation);
    drawBackground();
    tft.setTextColor(UPT_DISPLAY_FONT_PRIMARY_COLOR,
                     UPT_DISPLAY_BACKGROUND_COLOR);
    tft.setCursor(0, 0);

    spr.setColorDepth(16);
}

void showTextScreen(const char* text) {
    drawBackground();

#ifdef UPTDISPLAY_SHOW_GRID
    drawDevOverlay();
#endif /* UPTDISPLAY_SHOW_GRID */

    spr.loadFont(UPT_DISPLAY_FONT_MEDIUM);
    spr.setTextColor(UPT_DISPLAY_FONT_PRIMARY_COLOR,
                     UPT_DISPLAY_BACKGROUND_COLOR);

    const auto cursorX =
        static_cast<int16_t>(tft.width() / 2 - spr.textWidth(text) / 2);
    const auto cursorY =
        static_cast<int16_t>(tft.height() / 2 - spr.fontHeight() / 2);
    tft.setCursor(cursorX, cursorY);

    spr.printToSprite(text);
    spr.unloadFont();
}

void showInformationScreen(
    const std::vector<std::pair<std::string, std::string>>& information,
    const byte* image, const int16_t imageHeight, const int16_t imageWidth) {
    constexpr char title[12] = "INFORMATION";
    int16_t info_area_width_px;

    // Wipe screen
    drawBackground();

    // Define image location
    if (tft.rotation == 0) {
        // Vertical
        // Image is below the information
        drawXPos = 10;
        drawYPos = static_cast<int16_t>(tft.height() / 2);
        info_area_width_px = tft.width();
    } else {
        // Horizontal:
        // Image is next to the information
        drawXPos = static_cast<int16_t>(tft.width() / 2);
        drawYPos = 10;
        info_area_width_px = static_cast<int16_t>(tft.width() / 2);
    }

    // Print image
    tft.drawXBitmap(drawXPos, drawYPos, image, imageHeight, imageWidth,
                    UPT_DISPLAY_FONT_PRIMARY_COLOR);

    const auto x_value = static_cast<int16_t>(info_area_width_px / 2 + 5);
    int16_t y_line = 10;
    spr.loadFont(UPT_DISPLAY_FONT_MEDIUM);
    spr.setTextColor(UPT_DISPLAY_FONT_PRIMARY_COLOR,
                     UPT_DISPLAY_BACKGROUND_COLOR);

    const int f_height = tft.fontHeight();
    const int title_width_px = spr.textWidth(title);

    const auto xCursor =
        static_cast<int16_t>(info_area_width_px / 2 - title_width_px / 2);
    tft.setCursor(xCursor, y_line);
    spr.printToSprite(title);

    y_line += 25;

    for (const auto& info : information) {
        constexpr int x_key = 10;
        tft.setCursor(x_key, y_line);
        spr.printToSprite(info.first.c_str());

        tft.setCursor(x_value, y_line);
        spr.printToSprite(info.second.c_str());

        y_line = static_cast<int16_t>(y_line + f_height + 6);
    }

    spr.unloadFont();
}

void showSensorData(const SensorDisplayValues& data) {
    uint n_value = data.measurements.size();

    // Wipe screen
    drawBackground();

    if (n_value == 0 || n_value > 8) {
        showTextScreen("Sensor has no configured signals.");
        return;
    }
    if (tft.rotation == 0) {
        // Vertical screen has a top title
        drawVScreenTopTitle(data);
        drawVScreenLegend(data);
    } else {
        drawHScreenLegend(data);
    }

    SensorDisplayTile* tiles = getNTiles(n_value, tft.width(), tft.height());

    for (int i = 0; i < n_value; i++) {
        drawTile(tiles[i], data.measurements[i]);
        drawTileValue(tiles[i], data.measurements[i]);
    }

    delete tiles;

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

    if (tft.rotation == 0) {
        drawVScreenLegend(data);
    } else {
        drawHScreenLegend(data);
    }

    SensorDisplayTile* tiles = getNTiles(n_value, tft.width(), tft.height());

    for (int i = 0; i < n_value; i++) {
        eraseTileValue(tiles[i]);
        drawTileValue(tiles[i], data.measurements[i]);
    }

    delete tiles;

#ifdef UPTDISPLAY_SHOW_GRID
    drawDevOverlay();
#endif /* UPTDISPLAY_SHOW_GRID */
}

void drawDevOverlay() {
    constexpr int sep = 50;

    // Draw red grid lines
    for (int x = sep; x < tft.width(); x += sep) {
        tft.drawFastVLine(x, 0, tft.height(), TFT_RED);
    }
    for (int y = sep; y < tft.height(); y += sep) {
        tft.drawFastHLine(0, y, tft.width(), TFT_RED);
    }

    // Draw coordinate info
    for (int x = sep; x < tft.width(); x += sep) {
        char lbl[4];
        sprintf(lbl, "x%i", x);
        const auto cursorX =
            static_cast<int16_t>(x - tft.textWidth(lbl) / 2 + 1);
        tft.setCursor(cursorX, 0);
        tft.print(lbl);
    }
    for (int16_t y = sep; y < tft.height(); y += sep) {
        char lbl[4];
        sprintf(lbl, "y%i", y);
        const auto cursorY = static_cast<int16_t>(y - tft.fontHeight() / 2 + 1);
        tft.setCursor(0, cursorY);
        tft.print(lbl);
    }
}

void drawBackground() {
    tft.fillScreen(UPT_DISPLAY_BACKGROUND_COLOR);
}

void drawVScreenTopTitle(const SensorDisplayValues& data) {
    // Print sensor name
    spr.loadFont(UPT_DISPLAY_FONT_MEDIUM);
    spr.setTextColor(UPT_DISPLAY_FONT_PRIMARY_COLOR,
                     UPT_DISPLAY_BACKGROUND_COLOR);
    const auto cursorX = static_cast<int16_t>(
        tft.width() / 2 - spr.textWidth(data.sensorName.c_str()) / 2);
    tft.setCursor(cursorX, TILE_VERTICAL_TOP_OFFSET - spr.fontHeight());
    spr.printToSprite(data.sensorName.c_str());
    spr.unloadFont();
}

void drawVScreenLegend(const SensorDisplayValues& sensorData) {

    spr.loadFont(UPT_DISPLAY_FONT_MEDIUM);
    spr.setTextColor(UPT_DISPLAY_FONT_PRIMARY_COLOR,
                     UPT_DISPLAY_BACKGROUND_COLOR);

    int16_t cursorX = INTER_TILE_SPACING;
    const auto cursorY = static_cast<int16_t>(tft.height() - spr.fontHeight() -
                                              SCREEN_FRAME_MARGIN);

    // Print measurement time
    tft.setCursor(cursorX, cursorY);
    spr.printToSprite(sensorData.timeInfoStr.c_str());

    // Print sensor rank
    char rank[8];
    sprintf(rank, " %i/%i", sensorData.sensorRank,
            sensorData.numTrackedSensors);
    cursorX = static_cast<int16_t>(tft.width() - INTER_TILE_SPACING -
                                   spr.textWidth(rank));
    tft.setCursor(cursorX, cursorY);
    spr.printToSprite(rank);

    spr.unloadFont();
}

void drawHScreenLegend(const SensorDisplayValues& sensorData) {
    // Print sensor name
    spr.loadFont(UPT_DISPLAY_FONT_MEDIUM);
    spr.setTextColor(UPT_DISPLAY_FONT_PRIMARY_COLOR,
                     UPT_DISPLAY_BACKGROUND_COLOR);
    auto cursorX = static_cast<int16_t>(SCREEN_FRAME_MARGIN + 5);
    auto cursorY = static_cast<int16_t>(tft.height() - spr.fontHeight() -
                                        SCREEN_FRAME_MARGIN);

    tft.setCursor(cursorX, cursorY);
    char lbl[32];
    sprintf(lbl, "%s (%i/%i)", sensorData.sensorName.c_str(),
            sensorData.sensorRank, sensorData.numTrackedSensors);
    spr.printToSprite(lbl);
    const uint16_t sensorLabelWidth = spr.textWidth(lbl);
    spr.unloadFont();

    // Print measurement time
    spr.loadFont(UPT_DISPLAY_FONT_SMALL);
    cursorX = static_cast<int16_t>(cursorX + sensorLabelWidth + 5);
    cursorY = static_cast<int16_t>(tft.height() - spr.fontHeight() -
                                   SCREEN_FRAME_MARGIN);
    tft.setCursor(cursorX, cursorY);

    spr.printToSprite(sensorData.timeInfoStr.c_str());
    spr.unloadFont();
}

void drawTile(const SensorDisplayTile& tile,
              const core::Measurement& measurement) {
    // Draw Tile
    const auto rectX = static_cast<int16_t>(tile.tlx);
    const auto rectY = static_cast<int16_t>(tile.tly);
    const auto rectW = static_cast<int16_t>(tile.getWidth());
    const auto rectH = static_cast<int16_t>(tile.getHeight());
    tft.fillRoundRect(rectX, rectY, rectW, rectH, ROUNDED_CORNER_RADIUS,
                      UPT_DISPLAY_TILE_PRIMARY_COLOR);

    spr.setTextColor(UPT_DISPLAY_FONT_PRIMARY_COLOR,
                     UPT_DISPLAY_TILE_PRIMARY_COLOR);

    // Print signal description
    const auto cursorX = static_cast<int16_t>(tile.tlx + 10);
    const auto cursorY = static_cast<int16_t>(tile.tly + 5);
    tft.setCursor(cursorX, cursorY);
    switch (tile.type) {
        case TileType::SMALL:
            if (tft.rotation == 1) {
                // Load small font when in landscape
                spr.loadFont(UPT_DISPLAY_FONT_SMALL);
            } else {
                spr.loadFont(UPT_DISPLAY_FONT_MEDIUM);
            }
            spr.printToSprite(
                shortSignalDescription(measurement.signalType).c_str());
            break;
        case TileType::NARROW:
        case TileType::MEDIUM:
            spr.loadFont(UPT_DISPLAY_FONT_MEDIUM);
            spr.printToSprite(
                medSignalDescription(measurement.signalType).c_str());
            break;
        case TileType::LARGE:
            spr.loadFont(UPT_DISPLAY_FONT_MEDIUM);
            spr.printToSprite(
                longSignalDescription(measurement.signalType).c_str());
            break;
        default:
            tft.print("Error");
            break;
    }
    spr.unloadFont();
}

void eraseTileValue(const SensorDisplayTile& tile) {
    switch (tile.type) {
        case TileType::SMALL:
        case TileType::NARROW:
            spr.loadFont(UPT_DISPLAY_FONT_LARGE);
            break;
        case TileType::MEDIUM:
        case TileType::LARGE:
            spr.loadFont(UPT_DISPLAY_FONT_XLARGE);
            break;
        default:
            tft.print("Error");
            break;
    }

    const auto rectX = static_cast<int16_t>(tile.tlx);
    const auto rectY = static_cast<int16_t>(tile.tly + TILE_TITLE_OFFSET);
    const auto rectW = static_cast<int16_t>(tile.getWidth());
    const auto rectH = static_cast<int16_t>(tile.bry - rectY);

    tft.fillRoundRect(rectX, rectY, rectW, rectH, ROUNDED_CORNER_RADIUS,
                      UPT_DISPLAY_TILE_PRIMARY_COLOR);

    spr.unloadFont();
}

void drawTileValue(const SensorDisplayTile& tile,
                   const core::Measurement& measurement) {
    auto val = bufferValueAsString(measurement);
    uint xShiftValue = 0;
    uint yShiftValue = 0;

    switch (tile.type) {
        case TileType::SMALL:
            // Shift value if horizontal tile is too small to avoid overflow
            if (tft.rotation == 1) {
                xShiftValue = 15;
                yShiftValue = 2;
            }
        case TileType::NARROW:
            spr.loadFont(UPT_DISPLAY_FONT_LARGE);
            break;
        case TileType::MEDIUM:
        case TileType::LARGE:
            spr.loadFont(UPT_DISPLAY_FONT_XLARGE);
            break;
        default:
            tft.print("Error");
            break;
    }

    int valWidth = spr.textWidth(val.c_str());
    spr.setTextColor(colorOf(measurement), UPT_DISPLAY_TILE_PRIMARY_COLOR);

    // Offset height because of title

    const auto cursorX =
        static_cast<int16_t>(tile.getCx() - valWidth / 2 - xShiftValue);
    const auto cursorY = static_cast<int16_t>(tile.getCy() + TILE_TITLE_OFFSET -
                                              spr.fontHeight() + yShiftValue);

    tft.setCursor(cursorX, cursorY);
    spr.printToSprite(val.c_str());
    const int16_t valFontHeight = spr.fontHeight();
    spr.unloadFont();

    std::string unit = getGraphicSignalUnit(measurement.signalType);

    switch (tile.type) {
        case TileType::SMALL:
        case TileType::NARROW:
            spr.loadFont(UPT_DISPLAY_FONT_SMALL);
            break;
        case TileType::MEDIUM:
        case TileType::LARGE:
            spr.loadFont(UPT_DISPLAY_FONT_MEDIUM);
            break;
        default:
            tft.print("Error");
            break;
    }

    // Display units
    spr.setTextColor(UPT_DISPLAY_FONT_PRIMARY_COLOR,
                     UPT_DISPLAY_TILE_PRIMARY_COLOR);

    int16_t unitXPos, unitYPos;
    if (tft.rotation == 1) {
        unitXPos =
            static_cast<int16_t>(tile.getCx() + MEASUREMENT_VALUE_UNIT_SPACING +
                                 valWidth / 2 - xShiftValue);
        // Note: here we shift by 1/4 of font height because we need to ignore
        // the descendant part of the font and we estimate it at 1/4th of the
        // height.
        unitYPos = static_cast<int16_t>(cursorY + 3 * valFontHeight / 4 -
                                        3 * spr.fontHeight() / 4);
    } else {

        unitXPos = static_cast<int16_t>(tile.brx - ROUNDED_CORNER_RADIUS -
                                        spr.textWidth(unit.c_str()));
        unitYPos = static_cast<int16_t>(tile.bry - ROUNDED_CORNER_RADIUS -
                                        3 * spr.fontHeight() / 4);
    }

    tft.setCursor(unitXPos, unitYPos);

    spr.printToSprite(unit.c_str());
    spr.unloadFont();
}

std::string bufferValueAsString(const core::Measurement& measurement) {
    std::stringstream ss{};
    ss << std::fixed;  // use fixed notation for numbers.
    core::SignalType st = measurement.signalType;
    if (st == core::SignalType::TEMPERATURE_DEGREES_CELSIUS ||
        st == core::SignalType::TEMPERATURE_DEGREES_FARENHEIT ||
        st == core::SignalType::RELATIVE_HUMIDITY_PERCENTAGE ||
        st == core::SignalType::VELOCITY_METERS_PER_SECOND ||
        st == core::SignalType::GAS_CONCENTRATION_VOLUME_PERCENTAGE) {
        ss.precision(1);
    } else if (measurement.dataPoint.value < 10.0) {  // NOLINT(*-branch-clone)
        // A workaround because single char is not being displayed. can be
        // removed in the future
        ss << " ";
        ss.precision(0);
    } else {
        ss.precision(0);
    }
    ss << measurement.dataPoint.value;
    return ss.str();
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
            return UPT_DISPLAY_GREEN_COLOR;
        default:
            return 0;
    }
}

}  // namespace sensirion::upt::display
