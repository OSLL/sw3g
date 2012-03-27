#ifndef SIGNAL_STRENGTH_EVALUATOR_H
#define SIGNAL_STRENGTH_EVALUATOR_H

#include "core/evaluator.h"
#include "core/net_info.h"
#include "core/registries.h"

namespace fine {
    namespace impl {
        /**
          * Evaluates networks based on their average signal strength (in %).
          * Stronger signal implies better network.
          */
        class signal_strength_evaluator: public evaluator {
        public:
            /**
              * This evaluator is very simple. The rank of a network
              * is simply its average signal strength in %.
              */
            double rank(const network &net) {
                series ss_series = net_info::instance().param_values(
                            net, PARAM(parameters::SIGNAL_STRENGTH));
                return ss_series.mean();
            }

            double max_rank() {
                return 100;
            }

            double min_rank() {
                return 0;
            }
        };
    }
}

#endif // SIGNAL_STRENGTH_EVALUATOR_H
