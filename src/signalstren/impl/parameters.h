#ifndef PARAMETERS_H
#define PARAMETERS_H

#include "core/parameter.h"
#include "impl/units.h"

namespace fine {
    namespace impl {
        namespace parameters {
            static parameter SIGNAL_STRENGTH = parameter("Signal Strength", units::UNIT_POWER_IN_PERCENTS);
        }
    }
}

#endif // PARAMETERS_H
