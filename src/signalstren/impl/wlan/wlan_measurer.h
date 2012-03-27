#ifndef WLAN_MEASURER_H
#define WLAN_MEASURER_H

#include <map>
#include <iostream>
#include <istream>
#include <sstream>
#include <limits>

#include "core/measurer.h"
#include "core/registries.h"

#include "impl/config.h"

#include "util.h"

#include "wlan_scanner.h"

using namespace std;

namespace fine {
    namespace impl {
        class wlan_measurer: public measurer {
        public:
            wlan_measurer():
                cache_invalid_(true) {
                register_unit(PARAM(parameters::SIGNAL_STRENGTH),
                              UNIT(units::P_IN_PERCENTS));
            }
        private:
            // network id -> map of <parameter,param value>
            mutable map< string, map<parameter, double> > value_cache_;
            mutable bool cache_invalid_;

            /**
              * Refreshes all parameter values for each WLAN network.
              * Cache is invalidated (cache_invalid set to true) by the
              * wlan_scanner
              */
            void refresh() const {
                if (!cache_invalid_)
                    return;

                value_cache_.clear();

                string measure_result = exec("script/wlan_measure.sh");
                stringstream measure_stream(measure_result);
                cout << measure_result;
                while (measure_stream) {
                    string network_id;
                    measure_stream >> network_id;

                    if (measure_stream && (network_id.length() > 0)) {
                        double quality_in_percents;
                        measure_stream >> quality_in_percents;

                        // writing parameter values to a map
                        map<parameter, double> net_map;
                        net_map[PARAM(parameters::SIGNAL_STRENGTH)] = quality_in_percents;
                        value_cache_[network_id] = net_map;
                    }
                }

                cache_invalid_ = false;
            }

        protected:
            double value_internal(const network &net, const parameter &param) const {
                refresh();

                if (value_cache_.find(net.id()) == value_cache_.end()) {
                    // we have detected the network, but it has since then vanished.
                    // returning a NaN
                    return numeric_limits<double>::quiet_NaN();
                }
                return (value_cache_[net.id()])[param];
            }

            friend class wlan_scanner;
        };        
    }
}

#endif // WLAN_MEASURER_H
