// Example script showing mocked sensor data on the screen.

#include "Sensirion_upt_display.h"

SensorDisplayValues sdv;

/**
 * @brief This function generates artificial values
 * @param sdv: the SensorDisplayValues container
 * @param nSig: The number of signals to return
 */
void randomNSignalsSensor(SensorDisplayValues& sdv, uint8_t nSig) {
    char sensorname[16];
    sprintf(sensorname, "%s%d", "Sensor ", nSig);
    strncpy(sdv.sensorName, sensorname, 16);
    strncpy(sdv.timeInfoStr, "09:41:20", 21);
    sdv.numTrackedSensors = random(1, 10);
    sdv.sensorRank = random(1, sdv.numTrackedSensors);
    sdv.measurements.clear();

    for (size_t s = 0; s < nSig; s++) {
        Measurement m;
        m.signalType = static_cast<SignalType>(s + 1);
        m.dataPoint.t_offset = millis();
        m.dataPoint.value = random(10 * 1000) / 100.0;
        sdv.measurements.push_back(m);
    }
}

// The number of signals shown can be changed to simulate different sensors
int n_signal = 6;

void setup() {
    UptDisplay::init(landscape);

    UptDisplay::showTextScreen("Some simple text shown during setup...");
    delay(2000);

    // Display first values
    randomNSignalsSensor(sdv, n_signal);
    UptDisplay::showSensorData(sdv);
}

void loop() {
    randomNSignalsSensor(sdv, n_signal);
    // Only call refreshSensorData in order to redraw only necessary  sprites
    // (reduces flikering)
    UptDisplay::refreshSensorData(sdv);
    delay(500);
}
