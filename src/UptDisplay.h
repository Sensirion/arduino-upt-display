#ifndef UPT_DISPLAY_H
#define UPT_DISPLAY_H

#include "Sensirion_UPT_Core.h"
#include "TFT_eSPI.h"
#include <string>
#include <utility>
#include <vector>

using namespace sensirion::upt;

enum Orientation { portrait = 0, landscape = 1 };

/**
 * @brief A container for the measurements and sensor information
 */
struct SensorDisplayValues {
    char sensorName[32];
    char timeInfoStr[32];
    uint numTrackedSensors;
    uint sensorRank;
    std::vector<core::Measurement> measurements;
};

namespace UptDisplay {
/**
 * @brief initializes the library
 * @param orientation: defines the orientation of the screen
 */
void init(Orientation orientation);

/**
 * @brief Prints the given text centered on a blank background
 * @note The method does not check if the given text actually fits
 * @param text: the text to print
 */
void showTextScreen(const char* text);

/**
 * @brief Prints a the given image and key-value information.
 * The screen is split in half, with the first area being filled with the
 * key-value pairs and the second show the image
 * @param information: a vector of key-value pairs to be displayed
 * @param image: the image to display as a byte array.
 * The expected image is a bitmap (XBM).
 * @param imageHeight: the height of the image in pixels
 * @param imageWidth: the width of the image in pixels
 */
void showInformationScreen(
    const std::vector<std::pair<std::string, std::string>>& information,
    const byte* image, int16_t imageHeight, int16_t imageWidth);

/**
 * @brief Prints the given measurements as tiles.
 * @note Use refreshSensorData to refresh values in order to get a more
 * eye-pleasing refreshing.
 * @param sensorData: the measurements to show.
 */
void showSensorData(const SensorDisplayValues& sensorData);

/**
 * @brief Erases and prints the given measurements in the existing display.
 * @note Use showSensorData for your first call in order to draw the background
 * as well.
 * @param sensorData: the measurements to show.
 */
void refreshSensorData(const SensorDisplayValues& sensorData);

extern TFT_eSPI tft;
}  // namespace UptDisplay

#endif /* UPT_DISPLAY_H */
