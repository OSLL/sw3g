#ifndef NET_TYPE_REGISTRIES_H
#define NET_TYPE_REGISTRIES_H

#include "core/measurer.h"
#include "core/scanner.h"
#include "core/evaluator.h"
#include "core/registry.h"

namespace fine {
    /**
      * A registry of measurers, which measure network
      * parameters and average them over time. One network
      * type <-> one measurer
      */
    typedef registry<network_type, measurer> measurers;

    /**
      * A registry of scanners, which detect networks
      * of a given type. One network type <-> one scanner
      */
    typedef registry<network_type, scanner> scanners;

    /**
      * Registry of parameters. (Parameter name) <->
      * <-> parameter instance
      */
    typedef registry<string, parameter> params;

    /**
      * Registry of units. Unit name <-> unit instance
      */
    typedef registry<string, unit> uns;

    /**
      * A global list of evaluators, which rank networks
      * according to some criteria.
      */
    typedef global_list<evaluator> evaluators;

    //// Common shorthands for calling to global registries
    template <typename K, typename V>
    V& elem_for(const K &key) {
        return registry<K, V>::instance().get(key);
    }

    template <typename V>
    V& elem_for(const network_type &type) {
        return elem_for<network_type, V>(type);
    }

    template <typename V>
    V& elem_for(const string &name) {
        return elem_for<string, V>(name);
    }

    measurer &MEASURER(const network_type &type) {
        return elem_for<measurer>(type);
    }

    scanner &SCANNER(const network_type &type) {
        return elem_for<scanner>(type);
    }

    parameter &PARAM(const string &name) {
        return elem_for<parameter>(name);
    }

    unit &UNIT(const string &name) {
        return elem_for<unit>(name);
    }
}

#endif // NET_TYPE_REGISTRIES_H
