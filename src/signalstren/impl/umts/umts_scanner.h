#ifndef UMTS_SCANNER_H
#define UMTS_SCANNER_H

#include "core/scanner.h"
#include "util.h"

namespace fine {
    namespace impl {
        /**
          * Scanner for UMTS networks. Completely bogus for now.
          */
        class umts_scanner: public scanner {
        public:
            std::set<network> scan() {
                std::set<network> result;
                //if (exec("ifconfig -s | grep gprs0").length() > 0) {
                    result.insert(network("25520", "MegaFon", UMTS, true));
                //}

                return result;
            }
        };
    }
}

#endif // UMTS_SCANNER_H
