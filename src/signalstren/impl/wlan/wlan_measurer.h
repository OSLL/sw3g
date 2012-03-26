#ifndef WLAN_MEASURER_H
#define WLAN_MEASURER_H

#include <map>

#include "core/measurer.h"
#include "impl/parameters.h"
#include "impl/units.h"

using namespace std;

namespace fine {
    namespace impl {
        class wlan_measurer: public measurer {
        private:
            // TODO real measurement
            // TODO support for other parameters besides signal strength
            map<string, double> MEASUREMENTS_;
        public:
            wlan_measurer() {
                MEASUREMENTS_["shivu"] = -74;
                MEASUREMENTS_["SJCE_STUDENT"] = -35;
            }
        protected:
            double value_internal(const network &net, const parameter &param) const {
                // TODO perform real measurement
                if (param == parameters::SIGNAL_STRENGTH) {
                    if (MEASUREMENTS_.find(net.name()) == MEASUREMENTS_.end()) {
                        return -130;
                    } else {
                        return MEASUREMENTS_.at(net.name());
                    }
                }
            }

            const unit &measurement_unit(const parameter &param) const {
                if (param == parameters::SIGNAL_STRENGTH) {
                    return units::UNIT_POWER_IN_dBm;
                }
            }
        };
    }
}

#endif // WLAN_MEASURER_H
