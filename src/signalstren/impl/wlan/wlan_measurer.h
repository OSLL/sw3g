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
                              UNIT(units::PERCENT));
                register_unit(PARAM(parameters::SNR),
                              UNIT(units::DBM));
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

                stringstream measure_stream(exec("script/wlan_measure.sh"));
                while (measure_stream) {
                    string network_id = read_line(measure_stream);

                    if (measure_stream && (network_id.length() > 0)) {
                        // map for network parameter values
                        map<parameter, double> net_map;

                        // reading parameter values
                        double snr = read_double_opt(measure_stream);
                        double quality_in_percents = read_double_req(measure_stream);

                        // filling the map
                        net_map[PARAM(parameters::SIGNAL_STRENGTH)] = quality_in_percents;
                        net_map[PARAM(parameters::SNR)] = snr;
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

            bool has_measurement_internal(const network &net, const parameter &param) const {
                refresh();

                map<parameter, double> &net_meas_cache = value_cache_[net.id()];
                if (net_meas_cache.find(param) == net_meas_cache.end())
                    return false; // no such parameter

                // value is not undefined (NaN)
                return (net_meas_cache[param] == net_meas_cache[param]);
            }

            friend class wlan_scanner;
        };        
    }
}

#endif // WLAN_MEASURER_H
