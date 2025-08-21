#ifndef _UPT_DISPLAY_SIGNAL_TYPE_EXTENDED_H_
#define _UPT_DISPLAY_SIGNAL_TYPE_EXTENDED_H_

#include "Sensirion_UPT_Core.h"

using namespace sensirion::upt;

/*
 *   Extension of SignalType definition from UPT Core
 */

/* Add descriptions to signal types */
// Up to 27 characters
const char* longSignalDescription(core::SignalType signalType);
// Up to 20 characters
const char* medSignalDescription(core::SignalType signalType);
// Up to 10 characters
const char* shortSignalDescription(core::SignalType signalType);

// Icon maping for signal types
const char* signalIcon(core::SignalType signalType);

// Adds the unit for the given signal type to the character buffer
void getGraphicSignalUnit(char* buf, const core::SignalType& signalType);

#endif /* _UPT_DISPLAY_SIGNAL_TYPE_EXTENDED_H_ */