#ifndef NET_TYPE_REGISTRIES_H
#define NET_TYPE_REGISTRIES_H

#include "core/measurer.h"
#include "core/scanner.h"
#include "core/registry.h"

namespace fine {
    /**
      * A registry of measurers, which measure network
      * parameters and average them over time. One network
      * type <-> one measurer
      */
    class measurers: public registry<measurer> {
    };

    /**
      * A registry of scanners, which detect networks
      * of a given type. One network type <-> one scanner
      */
    class scanners: public registry<scanner> {
    };
}

#endif // NET_TYPE_REGISTRIES_H
