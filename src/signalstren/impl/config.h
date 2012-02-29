#ifndef CONFIG_H
#define CONFIG_H

/**
  * Specifies compile-time configuration of the FINE framework.
  */

// supported measurement units
#include "impl/units.h"

// supported evaluators
#include "impl/eval/single/signal_strength_evaluator.h"

// supported network types:
#include "impl/umts/reg.h"

#include "impl/wlan/reg.h"

#endif // CONFIG_H
