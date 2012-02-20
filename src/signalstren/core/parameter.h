#ifndef PARAMETER_H
#define PARAMETER_H

#include <string>

#include "core/unit.h"

namespace fine {
    using namespace std;

    /**
      * Defines a measurable network parameter, e.g. signal strength.
      * A parameter is immutable.
      */
    class parameter {
    private:
        string name_;
        string description_;
        unit base_unit_;
    public:
        /**
          * Creates a named measurable network parameter with a base
          * unit. Every measurement of such parameter will be converted
          * to that base unit if it is expressed in some other unit.
          */
        parameter(const string &name, const unit &base_unit):
            name_(name), description_(""), base_unit_(base_unit){
        }

        /**
          * Clones an existing parameter.
          */
        parameter(const parameter &param):
            name_(param.name_), description_(param.description_), base_unit_(param.base_unit_) {
        }

        bool operator==(const parameter &rhs) const {
            return rhs.name_ == name_ && rhs.base_unit_ == base_unit_;
        }

        bool operator<(const parameter &rhs) const {
            return rhs.name_ < name_ || (rhs.name_ == name_ && rhs.base_unit_ < base_unit_);
        }

        const string &name() const {
            return name_;
        }

        const string &description() const {
            return description_;
        }

        const unit &base_unit() const {
            return base_unit_;
        }
    };
}

#endif // PARAMETER_H
