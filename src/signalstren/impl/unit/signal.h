#ifndef SIGNAL_H
#define SIGNAL_H

#include <string>
#include "core/unit.h"

namespace fine {
    namespace impl {
        /**
          * Defines unit classes and names for measuring SIGNAL POWER
          */
        namespace units {
            /**
              * %
              */
            const string PERCENT = "%";
            /**
              * dBm (decibels above milliwatt).
              * dBm = 10 * log10(mW)
              */
            const string DBM = "dBm";

            class power_in_dbm_unit: public unit {
            public:
                power_in_dbm_unit():
                    unit(DBM) {
                }
            protected:
                bool is_convertible_to_internal(const unit &rhs) const {
                    if (rhs.name() == PERCENT) {
                        return true;
                    }
                    return false;
                }

                double convert_to_internal(const unit &rhs, double value) const {
                    if (rhs.name() == PERCENT) {
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
        }
    }
}

#endif // SIGNAL_H
