#ifndef MEASURER_H
#define MEASURER_H

#include <boost/shared_ptr.hpp>
#include <limits>

#include "core/network.h"
#include "core/parameter.h"
#include "core/unit.h"

namespace fine {
    using namespace boost;

    /**
      * Defines a measurer: something which can measure current value of a
      * network parameter.
      *
      * Default implementation also deals with the correspondence between measured
      * parameters and measurement units, subclasses don't have to override
      * <code>measurement_unit</code>; just to call <code>register_unit</code> in
      * their initialization code.
      */
    /*abstract*/ class measurer {
    public:
        /**
          * Returns whether parameter can be measured and actually has a recent
          * measurement.
          */
        bool has_measurement(const network &net, const parameter &param) const {
            return can_measure(param) && has_measurement_internal(net, param);
        }

        /**
          * Returns the current value of the parameter @p param of network @p net
          * in the base unit specified for parameter @p param.
          * If there is no current measurement, NaN is returned.
          * If the parameter and measurer units are incompatible (measurer units cannot
          * be converted to parameter units), NaN is returned.
          */
        double value(const network &net, const parameter &param) const {
            double value_in_measurer_units = value_internal(net, param);
            return measurement_unit(param).convert_to(param.base_unit(), value_in_measurer_units);
        }
    private:
        /**
          * Correspondence between network parameters and units of measurement
          * for them which are used by the measurer.
          */
        map< parameter, shared_ptr<const unit> > param_to_unit_;

        /**
          * Returns whether the measurer can give values of
          * the specified parameter.
          */
        bool can_measure(const parameter &param) const {
            return param_to_unit_.find(param) != param_to_unit_.end();
        }
    protected:
        /**
          * Returns the current value of the parameter @p param of network @p net
          * in the unit specified for the measurer
          */
        virtual double value_internal(const network &net, const parameter &param) const = 0;

        /**
          * Returns the measurement unit used by the measurer.
          * @param param - the parameter for which the unit is returned= 0;
          */
        virtual const unit &measurement_unit(const parameter &param) const {
            if (param_to_unit_.find(param) == param_to_unit_.end()) {
                throw logic_error("measurement unit for parameter not registered");
            }
            return *(param_to_unit_.at(param));
        }

        /**
          * Returns whether parameter actually has a recent measurement.
          */
        virtual bool has_measurement_internal(const network &net, const parameter &param) const = 0;

        /**
          * Registers measurement unit for a parameter
          * @param param - measured parameter
          * @param u - unit which is used by the measurer for values of @p param
          */
        void register_unit(const parameter &param, const unit &u) {
            param_to_unit_.insert(make_pair(param, shared_ptr<const unit>(&u)));
        }
    };
}

#endif // MEASURER_H
