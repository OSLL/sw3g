#ifndef REGISTRY_H
#define REGISTRY_H

#include <map>
#include <boost/shared_ptr.hpp>

#include "core/network.h"

using namespace std;
using namespace boost;

namespace fine {
    /**
      * Defines a registry of objects.
      * There may be only one object per network_type.
      */
    template<typename T>
    class registry {
    private:
        map< network_type, shared_ptr<T> > objects_;

        registry<T>() {
        }

        registry<T>(const registry<T>&) {
        }

        registry<T>& operator=(registry<T>&) {
        }
    public:
        /**
          * Associates the specified copy-constructible object
          * with a network_type.
          * If association already exists, the operation silently fails.
          *
          * @param type - network type
          * @param object - pointer to the object
          */
        void put(network_type type, T* object) {
            if (!is_registered(type)) {
                // does not yet exist, add item to the map
                objects_.insert(make_pair(type, shared_ptr<T>(object)));
            }
        }

        /**
          * Returns the object corresponding to the network_type.
          * @param type - network type
          * @return object for network of type @p type
          */
        T& get(network_type type) {
            pair< network_type, shared_ptr<T> > found = *(objects_.find(type));
            return *(found.second);
        }

        /**
          * Tells whether an object is registered for the specified
          * network type.
          * @param type - network type
          * @return true if registration exists
          */
        bool is_registered(network_type type) {
            return objects_.count(type) > 0;
        }

        /**
          * Returns a shared instance of the registry.
          */
        static registry<T>& instance() {
            static registry<T> the_instance;
            return the_instance;
        }
    };
}

#endif // REGISTRY_H
