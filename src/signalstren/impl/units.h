#ifndef UNITS_H
#define UNITS_H

#include <map>
#include <limits>

#include "core/unit.h"

using namespace std;

namespace fine {
    namespace impl {
        namespace units {
            /**
              * Power in %
              */
            static unit UNIT_POWER_IN_PERCENTS("Power (%)");

            class power_in_dbm_unit: public unit {
            public:
                power_in_dbm_unit():
                    unit("Power (dBm)") {
                }
            protected:
                bool is_convertible_to_internal(const unit &rhs) const {
                    if (rhs == UNIT_POWER_IN_PERCENTS) {
                        return true;
                    }
                    return false;
                }

                double convert_to_internal(const unit &rhs, double value) const {
                    if (rhs == UNIT_POWER_IN_PERCENTS) {
                        return convert(value);
                    }
                    throw logic_error("convert_to_internal called with invalid target unit");
                }
            private:
                /**
                  * Converts power in dBm to power in %.
                  * Allowed range of values (in dBm): -120..-20.
                  *
                  * @param dbm_cur - power in dBm
                  * @return power in %, or -1 if conversion unsuccessful
                  */
                double convert(double dbm_cur) const {
                    double value = 120 + dbm_cur;
                    if (value > 120)
                        value = 120;
                    else if (value < 0)
                        value = 0;
                    return value;
                }
            };

            /**
              * Power in decibels above milliwatt.
              * Convertible to power in %
              */
            static power_in_dbm_unit UNIT_POWER_IN_dBm;
        }
    }
}

#endif // UNITS_H
