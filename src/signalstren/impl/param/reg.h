#ifndef PARAM_REG_H
#define PARAM_REG_H

#include "core/parameter.h"
#include "core/registries.h"
#include "core/unit.h"

#include "signal.h"
#include "../unit/signal.h"

namespace fine {
    namespace impl {
        class param_init {
        public:
            /**
              * Adds parameters and units to the registry so that FINE network
              * can discover them.
              */
            param_init() {
                init_units();
                init_params();
            }
        private:
            /**
              * Adds units to the registry so that FINE network
              * can discover them.
              */
            void init_units() {
                // Power in %
                make_unconvertible_unit(units::P_IN_PERCENTS);
                // Power in dBm
                make_unit(new units::power_in_dbm_unit());

                // add new units before this line.
            }

            /**
              * Adds parameters to the registry so that FINE network
              * can discover them.
              */
            void init_params() {
                make_param(parameters::SIGNAL_STRENGTH, UNIT(units::P_IN_PERCENTS));

                // add new parameters before this line.
            }

            /**
              * Registers a new unconvertible unit with a specified name.
              * Repeatedly registering a unit with an existing name will
              * silently fail.
              * @param name - name for the unit
              */
            void make_unconvertible_unit(const string &name) {
                unit *u = new unit(name);
                make_unit(u);
            }

            /**
              * Registers the specified unit.
              * Registering a unit with an existing name will silently fail.
              * @param name - name for the unit
              */
            void make_unit(unit *u) {
                uns::instance().put(u->name(), u);
            }

            /**
              * Registers a new parameter.
              * @param name - name for the parameter
              * @param base_unit - base unit
              */
            void make_param(const string &name, const unit &base_unit) {
                parameter *param = new parameter(name, base_unit);
                params::instance().put(param->name(), param);
            }
        };

        static param_init _param_init;
    }
}

#endif // PARAM_REG_H
