#ifndef UNITS_H
#define UNITS_H

#include <map>

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
                }

                double convert_to_internal(const unit &rhs, double value) const {
                    if (rhs == UNIT_POWER_IN_PERCENTS) {
                        return convert(value);
                    }
                }
            private:
                /**
                  * Converts power in dBm to power in %.
                  * Allowed range of values (in dBm): -113..-10.
                  *
                  * @param dbm_cur - power in dBm
                  * @return power in %, or -1 if conversion unsuccessful
                  */
                double convert(double dbm_cur) const {
                    // Conversion table. Note that there are 95 entries corresponding
                    // to 0..94% of signal power.
                    // The conversion is slightly non-linear, e.g. -29 dBm->78%,
                    // -22->84, but -20->85 and not 86.
                    const int ITEM_COUNT = 95;
                    const double TABLE[ITEM_COUNT][2] = {
                        {-113, 0}, {-112, 1}, {-111, 2}, {-110, 3}, {-109, 4}, {-108, 5},
                        {-107, 6}, {-106, 7}, {-105, 8}, {-104, 9}, {-103, 10},{-102, 11},
                        {-101, 12},{-99, 13}, {-98, 14}, {-97, 15}, {-96, 16}, {-95, 17},
                        {-94, 18}, {-93, 19}, {-92, 20}, {-91, 21}, {-90, 22}, {-89, 23},
                        {-88, 24}, {-87, 25}, {-86, 26}, {-85, 27}, {-84, 28}, {-83, 29},
                        {-82, 30}, {-81, 31}, {-80, 32}, {-79, 33}, {-78, 34}, {-77, 35},
                        {-75, 36}, {-74, 37}, {-73, 38}, {-72, 39}, {-70, 40}, {-69, 41},
                        {-68, 42}, {-67, 43}, {-65, 44}, {-66, 45}, {-63, 46}, {-62, 47},
                        {-60, 48}, {-59, 49}, {-58, 50}, {-56, 51}, {-55, 52}, {-53, 53},
                        {-52, 54}, {-50, 55}, {-50, 56}, {-49, 57}, {-48, 58}, {-48, 59},
                        {-47, 60}, {-46, 61}, {-45, 62}, {-44, 63}, {-44, 64}, {-43, 65},
                        {-42, 66}, {-42, 67}, {-41, 68}, {-40, 69}, {-39, 70}, {-38, 71},
                        {-37, 72}, {-35, 73}, {-34, 74}, {-33, 75}, {-32, 76}, {-30, 77},
                        {-29, 78}, {-28, 79}, {-27, 80}, {-25, 81}, {-24, 82}, {-23, 83},
                        {-22, 84}, {-20, 85}, {-19, 86}, {-18, 87}, {-17, 88}, {-16, 89},
                        {-15, 90}, {-14, 91}, {-13, 92}, {-12, 93}, {-10, 94}};

                    for (int i = 0; i != ITEM_COUNT; ++i) {
                        double dbm = TABLE[i][0];
                        double percent = TABLE[i][1];
                        if (dbm == dbm_cur) {
                            // exact match
                            return percent;
                        } else if (dbm >= dbm_cur) {
                            // attempt linear interpolation between adjacent table entries
                            if (i == ITEM_COUNT - 1) {
                                // no interpolation, not enough data
                                return percent;
                            } else {
                                double dbm_next = TABLE[i + 1][0];
                                double percent_next = TABLE[i + 1][1];
                                return percent + (percent_next - percent) * (dbm_cur - dbm) /
                                       (dbm_next - dbm);
                            }
                        }
                    }

                    return -1;
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
