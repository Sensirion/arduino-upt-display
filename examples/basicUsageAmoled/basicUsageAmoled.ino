#ifndef LILYGO_TDISPLAY_AMOLED_SERIES
#define LILYGO_TDISPLAY_AMOLED_SERIES
#endif

#include "SensirionUptDisplay.h"
#include "AmoledDisplay.h"

using namespace sensirion::upt;

// Instantiate the Amoled display driver
display::AmoledDisplay amoledDisplay;
display::SensorDisplayValues sdv;

/**
 * @brief This function generates artificial values
 * @param displayValues: the SensorDisplayValues container
 * @param nSig: The number of signals to return
 */
void randomNSignalsSensor(display::SensorDisplayValues& displayValues,
                          const uint8_t nSig) {
    displayValues.sensorName = "Amoled Sensor " + std::to_string(nSig);
    displayValues.timeInfoStr = "12:34:56";
    displayValues.numTrackedSensors = random(1, 10);
    displayValues.sensorRank = random(1, displayValues.numTrackedSensors);
    displayValues.measurements.clear();

    for (size_t s = 0; s < nSig; s++) {
        core::Measurement m;
        m.signalType = static_cast<core::SignalType>(s + 1);
        m.dataPoint.t_offset = millis();
        m.dataPoint.value = random(10 * 1000) / 100.0;
        displayValues.measurements.push_back(m);
    }
}

// The number of signals shown can be changed to simulate different sensors
int n_signal = 6;

void setup() {
    // Initialize with the Amoled display driver
    display::init(amoledDisplay, display::landscape);

    display::showTextScreen("Amoled Display initializing...");
    delay(2000);

    // Display first values
    randomNSignalsSensor(sdv, n_signal);
    display::showSensorData(sdv);
}

void loop() {
    randomNSignalsSensor(sdv, n_signal);
    // Only call refreshSensorData to redraw only necessary sprites
    // (reduces flickering)
    display::refreshSensorData(sdv);
    delay(500);
}
