#include "SignalTypeExtended.h"

const char* longSignalDescription(const SignalType signalType) {
    switch (signalType) {
        case SignalType::TEMPERATURE_DEGREES_CELSIUS:
        case SignalType::TEMPERATURE_DEGREES_FARENHEIT:
            return "Temperature";
        case SignalType::RELATIVE_HUMIDITY_PERCENTAGE:
            return "Humidity";
        case SignalType::CO2_PARTS_PER_MILLION:
            return "Carbon Dioxide";
        case SignalType::HCHO_PARTS_PER_BILLION:
            return "Formaldehyde Concentration";
        case SignalType::PM1P0_MICRO_GRAMM_PER_CUBIC_METER:
            return "Particulate Matter PM1.0";
        case SignalType::PM2P5_MICRO_GRAMM_PER_CUBIC_METER:
            return "Particulate Matter PM2.5";
        case SignalType::PM4P0_MICRO_GRAMM_PER_CUBIC_METER:
            return "Particulate Matter PM4.0";
        case SignalType::PM10P0_MICRO_GRAMM_PER_CUBIC_METER:
            return "Particulate Matter PM10.0";
        case SignalType::VELOCITY_METERS_PER_SECOND:
            return "Velocity";
        case SignalType::RAW_VOC_INDEX:
            return "VOC Ticks";
        case SignalType::RAW_NOX_INDEX:
            return "NOX Ticks";
        case SignalType::VOC_INDEX:
            return "Volatile Organic Compounds Index";
        case SignalType::NOX_INDEX:
            return "Nitrous Oxide Index";
        case SignalType::GAS_CONCENTRATION_VOLUME_PERCENTAGE:
            return "Gas Concentration";
        default:
            return "UNDEFINED";
    }
}

const char* medSignalDescription(const SignalType signalType) {
    switch (signalType) {
        case SignalType::TEMPERATURE_DEGREES_CELSIUS:
        case SignalType::TEMPERATURE_DEGREES_FARENHEIT:
            return "Temperature";
        case SignalType::RELATIVE_HUMIDITY_PERCENTAGE:
            return "Humidity";
        case SignalType::CO2_PARTS_PER_MILLION:
            return "Carbon Dioxide";
        case SignalType::HCHO_PARTS_PER_BILLION:
            return "Formaldehyde Conc.";
        case SignalType::PM1P0_MICRO_GRAMM_PER_CUBIC_METER:
            return "Part. Matter PM1.0";
        case SignalType::PM2P5_MICRO_GRAMM_PER_CUBIC_METER:
            return "Part. Matter PM2.5";
        case SignalType::PM4P0_MICRO_GRAMM_PER_CUBIC_METER:
            return "Part. Matter PM4.0";
        case SignalType::PM10P0_MICRO_GRAMM_PER_CUBIC_METER:
            return "Part. Matter PM10.0";
        case SignalType::VELOCITY_METERS_PER_SECOND:
            return "Velocity";
        case SignalType::RAW_VOC_INDEX:
            return "VOC Ticks";
        case SignalType::RAW_NOX_INDEX:
            return "NOX Ticks";
        case SignalType::VOC_INDEX:
            return "Vol. Organic Comp.";
        case SignalType::NOX_INDEX:
            return "Nitrous Oxide";
        case SignalType::GAS_CONCENTRATION_VOLUME_PERCENTAGE:
            return "Gas Conc.";
        default:
            return "UNDEFINED";
    }
}

const char* shortSignalDescription(const SignalType signalType) {
    switch (signalType) {
        case SignalType::TEMPERATURE_DEGREES_CELSIUS:
        case SignalType::TEMPERATURE_DEGREES_FARENHEIT:
            return "Temp.";
        case SignalType::RELATIVE_HUMIDITY_PERCENTAGE:
            return "Humi.";
        case SignalType::CO2_PARTS_PER_MILLION:
            return "CO2";
        case SignalType::HCHO_PARTS_PER_BILLION:
            return "HCHO";
        case SignalType::PM1P0_MICRO_GRAMM_PER_CUBIC_METER:
            return "PM1.0";
        case SignalType::PM2P5_MICRO_GRAMM_PER_CUBIC_METER:
            return "PM2.5";
        case SignalType::PM4P0_MICRO_GRAMM_PER_CUBIC_METER:
            return "PM4.0";
        case SignalType::PM10P0_MICRO_GRAMM_PER_CUBIC_METER:
            return "PM10.0";
        case SignalType::VELOCITY_METERS_PER_SECOND:
            return "Vel.";
        case SignalType::RAW_VOC_INDEX:
            return "VOC Ticks";
        case SignalType::RAW_NOX_INDEX:
            return "NOX Ticks";
        case SignalType::VOC_INDEX:
            return "VOC Idx.";
        case SignalType::NOX_INDEX:
            return "NOX Idx.";
        case SignalType::GAS_CONCENTRATION_VOLUME_PERCENTAGE:
            return "Gas Conc.";
        default:
            return "UNDEFINED";
    }
}

const char* signalIcon(const SignalType signalType) {
    switch (signalType) {
        case SignalType::TEMPERATURE_DEGREES_CELSIUS:
        case SignalType::TEMPERATURE_DEGREES_FARENHEIT:
            return " ";
        case SignalType::RELATIVE_HUMIDITY_PERCENTAGE:
            return " ";
        case SignalType::CO2_PARTS_PER_MILLION:
            return " ";
        case SignalType::HCHO_PARTS_PER_BILLION:
            return " ";
        case SignalType::PM1P0_MICRO_GRAMM_PER_CUBIC_METER:
        case SignalType::PM2P5_MICRO_GRAMM_PER_CUBIC_METER:
        case SignalType::PM4P0_MICRO_GRAMM_PER_CUBIC_METER:
        case SignalType::PM10P0_MICRO_GRAMM_PER_CUBIC_METER:
            return " ";
        case SignalType::VELOCITY_METERS_PER_SECOND:
            return " ";
        case SignalType::VOC_INDEX:
        case SignalType::RAW_VOC_INDEX:
            return " ";
        case SignalType::NOX_INDEX:
        case SignalType::RAW_NOX_INDEX:
            return " ";
        case SignalType::GAS_CONCENTRATION_VOLUME_PERCENTAGE:
            return " ";
        default:
            return "  ";
    }
}

void getGraphicSignalUnit(char* buf, const SignalType& signalType) {
    switch (signalType) {
        case SignalType::TEMPERATURE_DEGREES_CELSIUS:
            strncpy(buf, "°C", 4);
            break;
        case SignalType::TEMPERATURE_DEGREES_FARENHEIT:
            strncpy(buf, "°F", 4);
            break;
        case SignalType::RELATIVE_HUMIDITY_PERCENTAGE:
            strncpy(buf, "% ", 3);
            break;
        case SignalType::CO2_PARTS_PER_MILLION:
            strncpy(buf, "ppm", 4);
            break;
        case SignalType::HCHO_PARTS_PER_BILLION:
            strncpy(buf, "ppb", 4);
            break;
        case SignalType::PM1P0_MICRO_GRAMM_PER_CUBIC_METER:
        case SignalType::PM2P5_MICRO_GRAMM_PER_CUBIC_METER:
        case SignalType::PM4P0_MICRO_GRAMM_PER_CUBIC_METER:
        case SignalType::PM10P0_MICRO_GRAMM_PER_CUBIC_METER:
            strncpy(buf, "µg/m³", 8);
            break;
        case SignalType::VELOCITY_METERS_PER_SECOND:
            strncpy(buf, "m/s", 6);
            break;
        case SignalType::RAW_VOC_INDEX:
        case SignalType::RAW_NOX_INDEX:
        case SignalType::VOC_INDEX:
        case SignalType::NOX_INDEX:
            strncpy(buf, "", 1);
            break;
        case SignalType::GAS_CONCENTRATION_VOLUME_PERCENTAGE:
            strncpy(buf, "% ", 3);
            break;
        default:
            strncpy(buf, "error", 6);
            break;
    }
}