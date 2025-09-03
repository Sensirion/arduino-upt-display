#include "SignalTypeExtended.h"

namespace sensirion::upt::display {

std::string longSignalDescription(const core::SignalType signalType) {
    switch (signalType) {
        case core::SignalType::TEMPERATURE_DEGREES_CELSIUS:
        case core::SignalType::TEMPERATURE_DEGREES_FARENHEIT:
            return "Temperature";
        case core::SignalType::RELATIVE_HUMIDITY_PERCENTAGE:
            return "Humidity";
        case core::SignalType::CO2_PARTS_PER_MILLION:
            return "Carbon Dioxide";
        case core::SignalType::HCHO_PARTS_PER_BILLION:
            return "Formaldehyde Concentration";
        case core::SignalType::PM1P0_MICRO_GRAMM_PER_CUBIC_METER:
            return "Particulate Matter PM1.0";
        case core::SignalType::PM2P5_MICRO_GRAMM_PER_CUBIC_METER:
            return "Particulate Matter PM2.5";
        case core::SignalType::PM4P0_MICRO_GRAMM_PER_CUBIC_METER:
            return "Particulate Matter PM4.0";
        case core::SignalType::PM10P0_MICRO_GRAMM_PER_CUBIC_METER:
            return "Particulate Matter PM10.0";
        case core::SignalType::VELOCITY_METERS_PER_SECOND:
            return "Velocity";
        case core::SignalType::RAW_VOC_INDEX:
            return "VOC Ticks";
        case core::SignalType::RAW_NOX_INDEX:
            return "NOX Ticks";
        case core::SignalType::VOC_INDEX:
            return "Volatile Organic Compounds Index";
        case core::SignalType::NOX_INDEX:
            return "Nitrous Oxide Index";
        case core::SignalType::GAS_CONCENTRATION_VOLUME_PERCENTAGE:
            return "Gas Concentration";
        default:
            return "UNDEFINED";
    }
}

std::string medSignalDescription(const core::SignalType signalType) {
    switch (signalType) {
        case core::SignalType::TEMPERATURE_DEGREES_CELSIUS:
        case core::SignalType::TEMPERATURE_DEGREES_FARENHEIT:
            return "Temperature";
        case core::SignalType::RELATIVE_HUMIDITY_PERCENTAGE:
            return "Humidity";
        case core::SignalType::CO2_PARTS_PER_MILLION:
            return "Carbon Dioxide";
        case core::SignalType::HCHO_PARTS_PER_BILLION:
            return "Formaldehyde Conc.";
        case core::SignalType::PM1P0_MICRO_GRAMM_PER_CUBIC_METER:
            return "Part. Matter PM1.0";
        case core::SignalType::PM2P5_MICRO_GRAMM_PER_CUBIC_METER:
            return "Part. Matter PM2.5";
        case core::SignalType::PM4P0_MICRO_GRAMM_PER_CUBIC_METER:
            return "Part. Matter PM4.0";
        case core::SignalType::PM10P0_MICRO_GRAMM_PER_CUBIC_METER:
            return "Part. Matter PM10.0";
        case core::SignalType::VELOCITY_METERS_PER_SECOND:
            return "Velocity";
        case core::SignalType::RAW_VOC_INDEX:
            return "VOC Ticks";
        case core::SignalType::RAW_NOX_INDEX:
            return "NOX Ticks";
        case core::SignalType::VOC_INDEX:
            return "Vol. Organic Comp.";
        case core::SignalType::NOX_INDEX:
            return "Nitrous Oxide";
        case core::SignalType::GAS_CONCENTRATION_VOLUME_PERCENTAGE:
            return "Gas Conc.";
        default:
            return "UNDEFINED";
    }
}

std::string shortSignalDescription(const core::SignalType signalType) {
    switch (signalType) {
        case core::SignalType::TEMPERATURE_DEGREES_CELSIUS:
        case core::SignalType::TEMPERATURE_DEGREES_FARENHEIT:
            return "Temp.";
        case core::SignalType::RELATIVE_HUMIDITY_PERCENTAGE:
            return "Humi.";
        case core::SignalType::CO2_PARTS_PER_MILLION:
            return "CO2";
        case core::SignalType::HCHO_PARTS_PER_BILLION:
            return "HCHO";
        case core::SignalType::PM1P0_MICRO_GRAMM_PER_CUBIC_METER:
            return "PM1.0";
        case core::SignalType::PM2P5_MICRO_GRAMM_PER_CUBIC_METER:
            return "PM2.5";
        case core::SignalType::PM4P0_MICRO_GRAMM_PER_CUBIC_METER:
            return "PM4.0";
        case core::SignalType::PM10P0_MICRO_GRAMM_PER_CUBIC_METER:
            return "PM10.0";
        case core::SignalType::VELOCITY_METERS_PER_SECOND:
            return "Vel.";
        case core::SignalType::RAW_VOC_INDEX:
            return "VOC Ticks";
        case core::SignalType::RAW_NOX_INDEX:
            return "NOX Ticks";
        case core::SignalType::VOC_INDEX:
            return "VOC Idx.";
        case core::SignalType::NOX_INDEX:
            return "NOX Idx.";
        case core::SignalType::GAS_CONCENTRATION_VOLUME_PERCENTAGE:
            return "Gas Conc.";
        default:
            return "UNDEFINED";
    }
}

std::string signalIcon(const core::SignalType signalType) {
    switch (signalType) {
        case core::SignalType::TEMPERATURE_DEGREES_CELSIUS:
        case core::SignalType::TEMPERATURE_DEGREES_FARENHEIT:
            return " ";
        case core::SignalType::RELATIVE_HUMIDITY_PERCENTAGE:
            return " ";
        case core::SignalType::CO2_PARTS_PER_MILLION:
            return " ";
        case core::SignalType::HCHO_PARTS_PER_BILLION:
            return " ";
        case core::SignalType::PM1P0_MICRO_GRAMM_PER_CUBIC_METER:
        case core::SignalType::PM2P5_MICRO_GRAMM_PER_CUBIC_METER:
        case core::SignalType::PM4P0_MICRO_GRAMM_PER_CUBIC_METER:
        case core::SignalType::PM10P0_MICRO_GRAMM_PER_CUBIC_METER:
            return " ";
        case core::SignalType::VELOCITY_METERS_PER_SECOND:
            return " ";
        case core::SignalType::VOC_INDEX:
        case core::SignalType::RAW_VOC_INDEX:
            return " ";
        case core::SignalType::NOX_INDEX:
        case core::SignalType::RAW_NOX_INDEX:
            return " ";
        case core::SignalType::GAS_CONCENTRATION_VOLUME_PERCENTAGE:
            return " ";
        default:
            return "  ";
    }
}

std::string getGraphicSignalUnit(const core::SignalType& signalType) {
    switch (signalType) {
        case core::SignalType::TEMPERATURE_DEGREES_CELSIUS:
            return "°C";
        case core::SignalType::TEMPERATURE_DEGREES_FARENHEIT:
            return "°F";
        case core::SignalType::RELATIVE_HUMIDITY_PERCENTAGE:
            return "% ";
        case core::SignalType::CO2_PARTS_PER_MILLION:
            return "ppm";
        case core::SignalType::HCHO_PARTS_PER_BILLION:
            return "ppb";
        case core::SignalType::PM1P0_MICRO_GRAMM_PER_CUBIC_METER:
        case core::SignalType::PM2P5_MICRO_GRAMM_PER_CUBIC_METER:
        case core::SignalType::PM4P0_MICRO_GRAMM_PER_CUBIC_METER:
        case core::SignalType::PM10P0_MICRO_GRAMM_PER_CUBIC_METER:
            return "µg/m³";
        case core::SignalType::VELOCITY_METERS_PER_SECOND:
            return "m/s";
        case core::SignalType::RAW_VOC_INDEX:
        case core::SignalType::RAW_NOX_INDEX:
        case core::SignalType::VOC_INDEX:
        case core::SignalType::NOX_INDEX:
            return "";
        case core::SignalType::GAS_CONCENTRATION_VOLUME_PERCENTAGE:
            return "% ";
        default:
            return "error";
    }
}

}  // namespace sensirion::upt::display
