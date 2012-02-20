#ifndef UMTS_SCANNER_H
#define UMTS_SCANNER_H

#include "core/scanner.h"
#include "util.h"

namespace fine {
    namespace impl {
        /**
          * Scanner for UMTS networks. Currently detects only whether network
          * is up or down, using ifconfig.
          * Reports "----" instead of the real network id.
          */
        class umts_scanner: public scanner {
        public:
            std::set<network> scan() {
                std::set<network> result;
                //if (exec("ifconfig -s | grep gprs0").length() > 0) {
                    //result.insert(network("25520", "MegaFon", UMTS));
                //}

                return result;
            }
        };
    }
}

#endif // UMTS_SCANNER_H
