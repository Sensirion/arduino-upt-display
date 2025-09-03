#ifndef _UPT_DISPLAY_SIGNAL_TYPE_EXTENDED_H_
#define _UPT_DISPLAY_SIGNAL_TYPE_EXTENDED_H_

#include "Sensirion_UPT_Core.h"

namespace sensirion::upt::display {

/*
 *   Extension of SignalType definition from UPT Core
 */

/* Add descriptions to signal types */
// Up to 27 characters
std::string longSignalDescription(core::SignalType signalType);
// Up to 20 characters
std::string medSignalDescription(core::SignalType signalType);
// Up to 10 characters
std::string shortSignalDescription(core::SignalType signalType);

// Icon maping for signal types
std::string signalIcon(core::SignalType signalType);

// Adds the unit for the given signal type to the character buffer
std::string getGraphicSignalUnit(const core::SignalType& signalType);

}  // namespace sensirion::upt::display

#endif /* _UPT_DISPLAY_SIGNAL_TYPE_EXTENDED_H_ */