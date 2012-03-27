#ifndef PARAMETER_H
#define PARAMETER_H

#include <string>

#include "core/unit.h"

namespace fine {
    using namespace std;

    /**
      * Defines a measurable network parameter, e.g. signal strength.
      * A parameter is immutable.
      *
      * Parameter is identified by its unique name; two parameters with
      * the same name are supposed to be equal.
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
            return rhs.name_ == name_;
        }

        bool operator<(const parameter &rhs) const {
            return rhs.name_ < name_;
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
