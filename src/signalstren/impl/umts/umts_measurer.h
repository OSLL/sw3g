#ifndef UMTS_MEASURER_H
#define UMTS_MEASURER_H

#include <map>
#include <limits>

#include "core/measurer.h"
#include "core/registries.h"

#include "impl/config.h"

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
                // TODO real measurement
                MEASUREMENTS_["MegaFon"] = -25.5;

                register_unit(PARAM(parameters::SIGNAL_STRENGTH),
                              UNIT(units::P_IN_dBm));
            }
        protected:
            double value_internal(const network &net, const parameter &param) const {
                if (param == PARAM(parameters::SIGNAL_STRENGTH)) {
                    return MEASUREMENTS_.at(net.name());
                }
                // silently fail. since this is not a real measurer implementation,
                // this is acceptable...
                return numeric_limits<double>::quiet_NaN();
            }
        };
    }
}

#endif // UMTS_MEASURER_H
