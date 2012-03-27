#ifndef UNIT_H
#define UNIT_H

#include <string>
#include <map>
#include <stdexcept>
#include <limits>

namespace fine {
    using namespace std;

    /**
      * Defines a measurement unit.
      * Measurement unit is identified by its unique name.
      */
    class unit {
    private:
        string name_;                
    public:
        /**
          * Converts value in current unit to value in @p rhs
          * unit.
          * Returns NaN if conversion is impossible.
          */
        double convert_to(const unit& rhs, double value) const {
            if (rhs.name_ == name_) {
                // converting to itself
                return value;
            } else {
                if (is_convertible_to(rhs)) {
                    return convert_to_internal(rhs, value);
                } else {
                    return numeric_limits<double>::quiet_NaN();
                }
            }
        }

        /**
          * Returns whether values in this unit can be converted
          * to another unit @p rhs using the convert_to() method.
          */
        bool is_convertible_to(const unit& rhs) const {
            return rhs.name_ == name_ || is_convertible_to_internal(rhs);
        }

        bool operator==(const unit &rhs) const {
            return rhs.name_ == name_;
        }

        bool operator<(const unit &rhs) const {
            return rhs.name_ < name_;
        }
    protected:
        /**
          * Converts value in current unit to another compatible (but
          * not the same) unit.
          */
        virtual double convert_to_internal(const unit&, double) const {
            // the unit is unconvertible - this is not meant to be called
            throw logic_error("convert_to_internal called for an unconvertible unit");
        }

        /**
          * Returns whether values in this unit can be converted
          * to a compatible (but not the same) unit.
          */
        virtual bool is_convertible_to_internal(const unit&) const {
            return false;
        }
    public:
        unit(const string &name):
            name_(name) {
        }

        unit(const unit &rhs):
            name_(rhs.name_) {
        }

        const string &name() const {
            return name_;
        }
    };

    /**
      * Unit which can be converted to another unit(s) using a table.
      */
    class table_convertible_unit: public unit {
    private:
        map< string, map<double, double> > conversions_;
    public:
        table_convertible_unit(const string &name):
            unit(name) {
        }

        /**
          * Adds a conversion table from this unit to unit @p rhs.
          * @param table - the correspondence between values of this unit and unit @p rhs
          * @param rhs - the unit to convert to
          */
        void add_conversion_table(const unit& rhs, const map<double, double> &table) {
            conversions_.insert(make_pair(rhs.name(), table));
        }

    private:
        bool is_convertible_to_internal(const unit &rhs) const {
            return (conversions_.find(rhs.name()) != conversions_.end());
        }

        double convert_to_internal(const unit& rhs, double value) const {
            pair< string, map<double, double> > found = *(conversions_.find(rhs.name()));
            map<double, double> &table = found.second;
            return table[value];
        }
    };
}

#endif // UNIT_H
