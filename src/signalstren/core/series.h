#ifndef SERIES_H
#define SERIES_H

#include <cmath>
#include <limits>
#include "core/parameter.h"
#include "core/net_info.h"

namespace fine {
    /**
      * Represents a snapshot of a series of parameter measurements.
      * Stores only the last measurement value, plus calculates mean
      * value, variance and stdev.
      */
    class series {
    private:
        // latest value
        double current_;

        // mean
        double old_m_;
        double new_m_;
        // variance * (N - 1)
        double old_s_;
        double new_s_;

        size_t count_;

        parameter param_;
    public:
        /**
          * Constructs an empty series for parameter @p param.
          */
        series(parameter &param):
            current_(numeric_limits<double>::quiet_NaN()), count_(0), param_(param) {
        }

        series(const series &rhs):
            current_(rhs.current_), old_m_(rhs.old_m_), new_m_(rhs.new_m_),
            old_s_(rhs.old_s_), new_s_(rhs.new_s_),
            count_(rhs.count_), param_(rhs.param_) {
        }

        series& operator=(const series& rhs) {
            current_ = rhs.current_;
            count_ = rhs.count_;
            param_ = rhs.param_;
            old_m_ = rhs.old_m_;
            new_m_ = rhs.new_m_;
            old_s_ = rhs.old_s_;
            new_s_ = rhs.new_s_;
            return *this;
        }

        /**
          * Returns the parameter
          */
        const parameter &for_param() const {
            return param_;
        }

        /**
          * Returns the last recorded value in the series.
          */
        double peek() const {
            return current_;
        }

        /**
          * Adds the value to the series.
          * @param value - value to add
          */
        void push(double value) {
            current_ = value;
            count_++;

            // See Knuth TAOCP vol 2, 3rd edition, page 232
            if (count_ == 1) {
                old_m_ = new_m_ = value;
                old_s_ = 0.0;
            } else {
                new_m_ = old_m_ + (value - old_m_) / count_;
                new_s_ = old_s_ + (value - old_m_) * (value - new_m_);

                // set up for next iteration
                old_m_ = new_m_;
                old_s_ = new_s_;
            }
        }

        /**
          * Returns the number of measurements.
          */
        size_t size() const {
            return count_;
        }

        /**
          * Returns the mean value of series.
          */
        double mean() const {
            return (count_ > 0) ? new_m_ : 0.0;
        }

        /**
          * Returns the variance of series.
          */
        double variance() const {
            return ( (count_ > 1) ? new_s_ / (count_ - 1) : 0.0 );
        }

        /**
          * Returns the standard deviation of series.
          */
        double stdev() const {
            return sqrt(variance());
        }

        /**
          * @return whether the series has recorded any measurements
          */
        bool empty() const {
            return (count_ > 0);
        }

        friend class net_info;
    };
}

#endif // SERIES_H
