#ifndef MEASURER_H
#define MEASURER_H

#include <limits>

#include "core/network.h"
#include "core/parameter.h"
#include "core/unit.h"

namespace fine {
    /**
      * Defines a measurer: something which can measure current value of a
      * network parameter.
      */
    /*abstract*/ class measurer {
    public:
        /**
          * Returns the current value of the parameter @p param of network @p net
          * in the base unit specified for parameter @p param.
          * If the parameter and measurer units are incompatible (measurer units cannot
          * be converted to parameter units), a NaN is returned.
          */
        double value(const network &net, const parameter &param) const {
            double value_in_measurer_units = value_internal(net, param);
            if (measurement_unit(param).is_convertible_to(param.base_unit())) {
                return measurement_unit(param).convert_to(param.base_unit(), value_in_measurer_units);
            } else {
                return std::numeric_limits<double>::quiet_NaN();
            }
        }
    protected:
        /**
          * Returns the current value of the parameter @p param of network @p net
          * in the unit specified for the measurer
          */
        virtual double value_internal(const network &net, const parameter &param) const = 0;

        /**
          * Returns the measurement unit used by the measurer.
          * @param param - the parameter for which the unit is returned
          */
        virtual const unit &measurement_unit(const parameter &param) const = 0;
    };
}

#endif // MEASURER_H
