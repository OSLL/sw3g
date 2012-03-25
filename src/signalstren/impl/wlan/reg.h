#ifndef WLANREG_H
#define WLANREG_H

/**
  * Registers WLAN network scanner and measurer with the FINE framework.
  */

#include "core/network.h"
#include "core/registries.h"

#include "wlan_measurer.h"
#include "wlan_scanner.h"

namespace fine {
    namespace impl {
        class wlan_init {
        public:
            /**
              * Adds WLAN network scanner and measurer to the registries
              * so that the framework can find them.
              */
            wlan_init() {
                scanners::instance().put(WLAN, new wlan_scanner());
                measurers::instance().put(WLAN, new wlan_measurer());
            }
        };

        static wlan_init _wlan_init;
    }
}

#endif // WLANREG_H
