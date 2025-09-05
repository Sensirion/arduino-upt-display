/* Example script showing an information page with an image/logo
 *
 * XBitmap is stored as an 8bit array and is generated using:
 * https://www.online-utility.org/image/convert/to/XBM
 *
 * The information is passed using a vector of key value pairs.
 * It allows to display configuration information
 */

#include "SensirionUptDisplay.h"
// Image is expected to be 150px by 150px
#include "images/tux_image_150_px.h"

using namespace sensirion::upt;

void setup() {
    display::init(display::landscape);
    std::vector<std::pair<std::string, std::string>> infos = {
        {"Version", "1.0.0"}, {"Temp. unit", "celsius"}, {"WiFi", "disabled"}};

    display::showInformationScreen(infos, tux, 150, 150);
}

void loop() {
    // Nothing is needed here
    delay(500);
}
