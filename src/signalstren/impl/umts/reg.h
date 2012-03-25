#ifndef UMTSREG_H
#define UMTSREG_H

/**
  * Registers UMTS network scanner and measurer with the FINE framework.
  */

#include "core/network.h"
#include "core/registries.h"

#include "umts_measurer.h"
#include "umts_scanner.h"

namespace fine {
    namespace impl {
        class umts_init {
        public:
            /**
              * Adds UMTS network scanner and measurer to the registries
              * so that the framework can find them.
              */
            umts_init() {
                scanners::instance().put(UMTS, new umts_scanner());
                measurers::instance().put(UMTS, new umts_measurer());
            }
        };

        static umts_init _umts_init;
    }
}

#endif // UMTSREG_H
