#include "UptDisplay.h"
#include "SignalTypeExtended.h"
#include <SensirionColors.h>
#include <Tiling.h>
#include <fonts/DefaultFont.h>

#define ROUNDED_CORNER_RADIUS 10
#define SCREEN_FRAME_MARGIN 2
#define MEASUREMENT_VALUE_UNIT_SPACING 5
#define TILE_TITLE_OFFSET 20

using namespace sensirion::upt;

// Dev grid overlay can be enabled by compiling with the flag
// UPTDISPLAY_SHOW_GRID
void drawDevOverlay();

void drawBackground();
void drawVScreenTopTitle(const SensorDisplayValues& sensorData);
void drawVScreenLegend(const SensorDisplayValues& sensorData);
void drawHScreenLegend(const SensorDisplayValues& sensorData);
void drawTile(SensorDisplayTile tile, const core::Measurement& measurement);
void drawTileValue(SensorDisplayTile tile, const core::Measurement& measurement);
void eraseTileValue(SensorDisplayTile tile);

/* Buffer a signal as a string */
void bufferValueAsString(char* buf, const core::Measurement& measurement);

/* Get color with which a signal should be displayed */
uint32_t colorOf(const core::Measurement& measurement);

TFT_eSPI UptDisplay::tft;
auto spr = TFT_eSprite(&UptDisplay::tft);

static int16_t drawXPos;
static int16_t drawYPos;

// ReSharper disable once CppDeclaratorNeverUsed
static auto TAG = "VIZ";

void UptDisplay::init(const Orientation orientation) {
    tft.init();
    tft.setTextWrap(false);
    tft.setRotation(orientation);
    drawBackground();
    tft.setTextColor(UPT_DISPLAY_FONT_PRIMARY_COLOR,
                     UPT_DISPLAY_BACKGROUND_COLOR);
    tft.setCursor(0, 0);

    spr.setColorDepth(16);
}

void UptDisplay::showTextScreen(const char* text) {
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

void UptDisplay::showInformationScreen(
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

void UptDisplay::showSensorData(const SensorDisplayValues& data) {
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

void UptDisplay::refreshSensorData(const SensorDisplayValues& data) {
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
    for (int x = sep; x < UptDisplay::tft.width(); x += sep) {
        UptDisplay::tft.drawFastVLine(x, 0, UptDisplay::tft.height(), TFT_RED);
    }
    for (int y = sep; y < UptDisplay::tft.height(); y += sep) {
        UptDisplay::tft.drawFastHLine(0, y, UptDisplay::tft.width(), TFT_RED);
    }

    // Draw coordinate info
    for (int x = sep; x < UptDisplay::tft.width(); x += sep) {
        char lbl[4];
        sprintf(lbl, "x%i", x);
        const auto cursorX =
            static_cast<int16_t>(x - UptDisplay::tft.textWidth(lbl) / 2 + 1);
        UptDisplay::tft.setCursor(cursorX, 0);
        UptDisplay::tft.print(lbl);
    }
    for (int16_t y = sep; y < UptDisplay::tft.height(); y += sep) {
        char lbl[4];
        sprintf(lbl, "y%i", y);
        const auto cursorY =
            static_cast<int16_t>(y - UptDisplay::tft.fontHeight() / 2 + 1);
        UptDisplay::tft.setCursor(0, cursorY);
        UptDisplay::tft.print(lbl);
    }
}

void drawBackground() {
    UptDisplay::tft.fillScreen(UPT_DISPLAY_BACKGROUND_COLOR);
}

void drawVScreenTopTitle(const SensorDisplayValues& data) {
    // Print sensor name
    spr.loadFont(UPT_DISPLAY_FONT_MEDIUM);
    spr.setTextColor(UPT_DISPLAY_FONT_PRIMARY_COLOR,
                     UPT_DISPLAY_BACKGROUND_COLOR);
    const auto cursorX = static_cast<int16_t>(
        UptDisplay::tft.width() / 2 - spr.textWidth(data.sensorName) / 2);
    UptDisplay::tft.setCursor(cursorX,
                              TILE_VERTICAL_TOP_OFFSET - spr.fontHeight());
    spr.printToSprite(data.sensorName);
    spr.unloadFont();
}

void drawVScreenLegend(const SensorDisplayValues& sensorData) {

    spr.loadFont(UPT_DISPLAY_FONT_MEDIUM);
    spr.setTextColor(UPT_DISPLAY_FONT_PRIMARY_COLOR,
                     UPT_DISPLAY_BACKGROUND_COLOR);

    auto cursorX = INTER_TILE_SPACING;
    const auto cursorY = static_cast<int16_t>(
        UptDisplay::tft.height() - spr.fontHeight() - SCREEN_FRAME_MARGIN);

    // Print measurement time
    UptDisplay::tft.setCursor(cursorX, cursorY);
    spr.printToSprite(sensorData.timeInfoStr);

    // Print sensor rank
    char rank[8];
    sprintf(rank, " %i/%i", sensorData.sensorRank,
            sensorData.numTrackedSensors);
    cursorX = static_cast<int16_t>(UptDisplay::tft.width() -
                                   INTER_TILE_SPACING - spr.textWidth(rank));
    UptDisplay::tft.setCursor(cursorX, cursorY);
    spr.printToSprite(rank);

    spr.unloadFont();
}

void drawHScreenLegend(const SensorDisplayValues& sensorData) {
    // Print sensor name
    spr.loadFont(UPT_DISPLAY_FONT_MEDIUM);
    spr.setTextColor(UPT_DISPLAY_FONT_PRIMARY_COLOR,
                     UPT_DISPLAY_BACKGROUND_COLOR);
    auto cursorX = static_cast<int16_t>(SCREEN_FRAME_MARGIN + 5);
    auto cursorY = static_cast<int16_t>(UptDisplay::tft.height() -
                                        spr.fontHeight() - SCREEN_FRAME_MARGIN);

    UptDisplay::tft.setCursor(cursorX, cursorY);
    char lbl[32];
    sprintf(lbl, "%s (%i/%i)", sensorData.sensorName, sensorData.sensorRank,
            sensorData.numTrackedSensors);
    spr.printToSprite(lbl);
    int sensorLabelWidth = spr.textWidth(lbl);
    spr.unloadFont();

    // Print measurement time
    spr.loadFont(UPT_DISPLAY_FONT_SMALL);
    cursorX += sensorLabelWidth + 5;
    cursorY = static_cast<int16_t>(UptDisplay::tft.height() - spr.fontHeight() -
                                   SCREEN_FRAME_MARGIN);
    UptDisplay::tft.setCursor(cursorX, cursorY);

    spr.printToSprite(sensorData.timeInfoStr);
    spr.unloadFont();
}

void drawTile(SensorDisplayTile tile, const core::Measurement& measurement) {
    // Draw Tile
    const auto rectX = static_cast<int16_t>(tile.tlx);
    const auto rectY = static_cast<int16_t>(tile.tly);
    const auto rectW = static_cast<int16_t>(tile.getWidth());
    const auto rectH = static_cast<int16_t>(tile.getHeight());
    UptDisplay::tft.fillRoundRect(rectX, rectY, rectW, rectH,
                                  ROUNDED_CORNER_RADIUS,
                                  UPT_DISPLAY_TILE_PRIMARY_COLOR);

    spr.setTextColor(UPT_DISPLAY_FONT_PRIMARY_COLOR,
                     UPT_DISPLAY_TILE_PRIMARY_COLOR);

    // Print signal description
    const auto cursorX = static_cast<int16_t>(tile.tlx + 10);
    const auto cursorY = static_cast<int16_t>(tile.tly + 5);
    UptDisplay::tft.setCursor(cursorX, cursorY);
    switch (tile.type) {
        case TileType::SMALL:
            if (UptDisplay::tft.rotation == 1) {
                // Load small font when in landscape
                spr.loadFont(UPT_DISPLAY_FONT_SMALL);
            } else {
                spr.loadFont(UPT_DISPLAY_FONT_MEDIUM);
            }
            spr.printToSprite(shortSignalDescription(measurement.signalType));
            break;
        case TileType::NARROW:
        case TileType::MEDIUM:
            spr.loadFont(UPT_DISPLAY_FONT_MEDIUM);
            spr.printToSprite(medSignalDescription(measurement.signalType));
            break;
        case TileType::LARGE:
            spr.loadFont(UPT_DISPLAY_FONT_MEDIUM);
            spr.printToSprite(longSignalDescription(measurement.signalType));
            break;
        default:
            UptDisplay::tft.print("Error");
            break;
    }
    spr.unloadFont();
}

void eraseTileValue(SensorDisplayTile tile) {
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
            UptDisplay::tft.print("Error");
            break;
    }

    const auto rectX = static_cast<int16_t>(tile.tlx);
    const auto rectY = static_cast<int16_t>(tile.tly + TILE_TITLE_OFFSET);
    const auto rectW = static_cast<int16_t>(tile.getWidth());
    const auto rectH = static_cast<int16_t>(tile.bry - rectY);

    UptDisplay::tft.fillRoundRect(rectX, rectY, rectW, rectH,
                                  ROUNDED_CORNER_RADIUS,
                                  UPT_DISPLAY_TILE_PRIMARY_COLOR);

    spr.unloadFont();
}

void drawTileValue(SensorDisplayTile tile, const core::Measurement& measurement) {
    char val[32];  // Numerical value of measurement
    bufferValueAsString(val, measurement);
    uint xShiftValue = 0;
    uint yShiftValue = 0;

    switch (tile.type) {
        case TileType::SMALL:
            // Shift value if horizontal tile is too small to avoid overflow
            if (UptDisplay::tft.rotation == 1) {
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
            UptDisplay::tft.print("Error");
            break;
    }

    int valWidth = spr.textWidth(val);
    spr.setTextColor(colorOf(measurement), UPT_DISPLAY_TILE_PRIMARY_COLOR);

    // Offset height because of title

    const auto cursorX =
        static_cast<int16_t>(tile.getCx() - valWidth / 2 - xShiftValue);
    const auto cursorY = static_cast<int16_t>(tile.getCy() + TILE_TITLE_OFFSET -
                                              spr.fontHeight() + yShiftValue);

    UptDisplay::tft.setCursor(cursorX, cursorY);
    spr.printToSprite(val);
    const int16_t valFontHeight = spr.fontHeight();
    spr.unloadFont();

    char unit[8];  // Unit of measurement
    getGraphicSignalUnit(unit, measurement.signalType);

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
            UptDisplay::tft.print("Error");
            break;
    }

    // Display units
    spr.setTextColor(UPT_DISPLAY_FONT_PRIMARY_COLOR,
                     UPT_DISPLAY_TILE_PRIMARY_COLOR);

    int16_t unitXPos, unitYPos;
    if (UptDisplay::tft.rotation == 1) {
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
                                        spr.textWidth(unit));
        unitYPos = static_cast<int16_t>(tile.bry - ROUNDED_CORNER_RADIUS -
                                        3 * spr.fontHeight() / 4);
    }

    UptDisplay::tft.setCursor(unitXPos, unitYPos);

    spr.printToSprite(unit);
    spr.unloadFont();
}

void bufferValueAsString(char* buf, const core::Measurement& measurement) {
    core::SignalType st = measurement.signalType;
    if (st == core::SignalType::TEMPERATURE_DEGREES_CELSIUS ||
        st == core::SignalType::TEMPERATURE_DEGREES_FARENHEIT ||
        st == core::SignalType::RELATIVE_HUMIDITY_PERCENTAGE ||
        st == core::SignalType::VELOCITY_METERS_PER_SECOND ||
        st == core::SignalType::GAS_CONCENTRATION_VOLUME_PERCENTAGE) {
        sprintf(buf, "%.1f", measurement.dataPoint.value);
    } else if (measurement.dataPoint.value < 10.0) {
        // A workaround because single char is not being displayed. can be
        // removed in the future
        sprintf(buf, "%.0f.0", measurement.dataPoint.value);
    } else {
        sprintf(buf, "%.0f", measurement.dataPoint.value);
    }
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
        case core::SignalType::HCHO_PARTS_PER_BILLION:  // NOLINT(*-branch-clone)
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
