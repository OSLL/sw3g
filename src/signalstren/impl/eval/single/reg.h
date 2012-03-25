#ifndef SIGNAL_STRENGTH_EVAL_REG_H
#define SIGNAL_STRENGTH_EVAL_REG_H

/**
  * Registers Signal Strength Evaluator with the FINE framework.
  */

#include "core/network.h"
#include "core/registries.h"

#include "signal_strength_evaluator.h"

namespace fine {
    namespace impl {
        class signal_strength_eval_init {
        public:
            /**
              * Adds Signal Strength Evaluator to the Evaluator list
              * so that the framework can find it.
              */
            signal_strength_eval_init() {
                evaluators::instance().put(new signal_strength_evaluator());
            }
        };

        static signal_strength_eval_init _signal_strength_eval_init;
    }
}

#endif // SIGNAL_STRENGTH_EVAL_REG_H
