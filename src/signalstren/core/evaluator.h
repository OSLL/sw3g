#ifndef EVALUATOR_H
#define EVALUATOR_H

#include "core/network.h"

using namespace std;

namespace fine {
    /**
      * Defines interface for all network evaluators.
      */
    /*abstract*/ class evaluator {
    public:
        /**
          * @return Name for the evaluator
          */
        virtual string name() const = 0;

        /**
          * Assigns network a rank between min_rank() and max_rank().
          * Bigger rank is better.
          * @return rank for the network
          */
        virtual double rank(const network &net) = 0;

        /**
          * @return value for worst network
          */
        virtual double min_rank() = 0;

        /**
          * @return value for best network
          */
        virtual double max_rank() = 0;
    };
}

#endif // EVALUATOR_H
