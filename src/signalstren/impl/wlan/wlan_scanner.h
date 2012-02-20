#ifndef WLAN_SCANNER_H
#define WLAN_SCANNER_H

#include "core/scanner.h"
#include "util.h"

namespace fine {
    namespace impl {
        /**
          * Scanner for WLAN networks
          */
        class wlan_scanner: public scanner {
        public:
            std::set<network> scan() {
                std::set<network> result;
                result.insert(network("00:1E:58:B8:AB:A3", "shivu", WLAN));
                result.insert(network("00:18:E7:8C:B6:D2", "SJCE_STUDENT", WLAN));
                return result;
            }
        };
    }
}

#endif // WLAN_SCANNER_H
