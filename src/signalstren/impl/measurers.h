#ifndef MEASURERS_H
#define MEASURERS_H

#include <map>
#include <boost/shared_ptr.hpp>

#include "core/measurer.h"
#include "impl/umts/umts_measurer.h"
#include "impl/wlan/wlan_measurer.h"

namespace fine {
    namespace impl {
        using namespace std;
        using namespace boost;

        class measurers: public measurer_instance {
        private:
            map< network_type, shared_ptr<measurer> > measurers_;

            measurers() {
                measurers_.insert(make_pair(UMTS, shared_ptr<measurer>(new umts_measurer())));
                measurers_.insert(make_pair(WLAN, shared_ptr<measurer>(new wlan_measurer())));
            }

            measurers(const measurers&) {
            }

            measurers& operator=(measurers&) {
            }
        public:
            measurer& get(network_type type) {
                pair< network_type, shared_ptr<measurer> > found = *(measurers_.find(type));
                return *(found.second);
            }

            static measurers& instance() {
                static measurers the_instance;
                return the_instance;
            }
        };
    }
}

#endif // MEASURERS_H
