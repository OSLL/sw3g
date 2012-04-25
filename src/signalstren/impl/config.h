#ifndef CONFIG_H
#define CONFIG_H

/**
  * Specifies compile-time configuration of the FINE framework.
  */

//// Supported parameters & measurement units
// IMPORTANT: Static initialization of parameters and units must
// occur before initialization of network types, because these
// would likely internally register a <parameter->unit> map on
// initialization.
#include "impl/param/reg.h"

//// Supported evaluators
#include "impl/eval/single/reg.h"
// Add new new evaluators before this line

//// Supported network types:
#include "impl/umts/reg.h"
#include "impl/wlan/reg.h"
// Add new new network types before this line

//// Tweakable parameters
#include "core/net_info.h"

// Network Info cache lifetime
const int fine::net_info::CACHE_LIFETIME = 600;

// Timer for networks vanishing from scan
const size_t fine::net_info::VANISHING_TIMEOUT = 5;

#include "core/vote.h"
// Minimum network age to participate in voting
const size_t fine::vote::MINIMAL_NETWORK_AGE = 1;

#endif // CONFIG_H
