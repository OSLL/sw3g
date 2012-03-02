#ifndef UMTS_MEASURER_H
#define UMTS_MEASURER_H

#include <map>

#include "core/measurer.h"
#include "impl/parameters.h"

using namespace std;

namespace fine {
    namespace impl {
        class umts_measurer: public measurer {
        private:
            // TODO real measurement
            // TODO support for other parameters besides signal strength
            map<string, double> MEASUREMENTS_;
        public:
            umts_measurer() {
                MEASUREMENTS_["MegaFon"] = -25.5;
            }
        protected:
            double value_internal(const network &net, const parameter &param) const {
                if (param == parameters::SIGNAL_STRENGTH) {
                    return MEASUREMENTS_.at(net.name());
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

#endif // UMTS_MEASURER_H
