#ifndef SCANNER_H
#define SCANNER_H

/**
  * @file scanner.h
  */

#include <set>
#include "core/network.h"

namespace fine {
    /**
      * Defines interface for a neighboring network detector, called `scanner'.
      * Scanner reports network identifiers (e.g. MAC for WLAN, PLMN code for 3G),
      * which should be recognizable to the {@link measurer measurers}.
      */
    /*abstract*/ class scanner {
    public:
        /**
          * Detects neighboring networks.
          *
          * @return list of neighboring networks
          */
        virtual std::set<network> scan() = 0;
    };
}

#endif // SCANNER_H
