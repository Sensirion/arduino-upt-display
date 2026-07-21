#ifndef UPT_DISPLAY_H
#define UPT_DISPLAY_H

#include "IDisplay.h"
#include "Sensirion_UPT_Core.h"
#include <string>
#include <utility>
#include <vector>

namespace sensirion::upt::display {

/**
 * @brief A container for the measurements and sensor information
 */
struct SensorDisplayValues {
    std::string sensorName;
    std::string timeInfoStr;
    uint numTrackedSensors;
    uint sensorRank;
    std::vector<core::Measurement> measurements;
};

/**
 * @brief initializes the library
 * @param orientation: defines the orientation of the screen
 */
void init(Orientation orientation);

/**
 * @brief initializes the library with a custom display driver
 * @param display: the display driver to use
 * @param orientation: defines the orientation of the screen
 */
void init(IDisplay& display, Orientation orientation);

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

// Deprecated: use IDisplay interface instead
// extern TFT_eSPI tft;

}  // namespace sensirion::upt::display

#endif /* UPT_DISPLAY_H */
