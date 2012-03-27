#ifndef NET_INFO_H
#define NET_INFO_H

#include <set>
#include <map>
#include <vector>
#include <algorithm>
#include <boost/noncopyable.hpp>

#include "core/network.h"
#include "core/parameter.h"
#include "core/series.h"

#include "core/registries.h"

// TODO remove this when a real list of parameters will be read dynamically
// instead of being constant
#include "impl/param/reg.h"

namespace fine {
    using namespace std;

    /**
      * Stores Network Information: which networks are available,
      * what parameters are measured for each, what parameter
      * values are.
      *
      * TODO: Stored information is persisted across different runs of the
      * `signalstren' application.
      */
    class net_info: public boost::noncopyable {
    private:
        net_info() {
            // TODO: read this info from somewhere
            watched_params_.insert(PARAM(impl::parameters::SIGNAL_STRENGTH));

            // TODO: read information from persisted storage
            vector<series> v1;
            series s1(PARAM(impl::parameters::SIGNAL_STRENGTH));
            s1.push(53); // %
            s1.push(67); // %
            v1.push_back(s1);
            parameter_values_[network("00:50:18:64:1E:88", "__ROUTER__", WLAN, false)] = v1;

            vector<series> v2;
            series s2(PARAM(impl::parameters::SIGNAL_STRENGTH));
            s2.push(87); // %
            s2.push(75); // %
            s2.push(32); // %
            v2.push_back(s2);
            parameter_values_[network("00:18:E7:8C:B6:D5", "SJCE_STUDENT", WLAN, true)] = v2;
        }

        ~net_info() {
            // TODO: persist network information
        }

        map< network,vector<series> > parameter_values_;
        set<parameter> watched_params_;
        set<network> networks_;
    public:
        void dump() {
            for (map< network, vector<series> >::iterator i = parameter_values_.begin(); i != parameter_values_.end(); ++i) {
                pair<network, vector<series> > elem = *i;
                network &net = elem.first;
                cout << "NETWORK - " << net.id() << "\n" <<
                        net.name() << "\n" << net.type() << "\n" <<
                        "    SERIES:\n";
                vector<series> &vec = elem.second;
                for (vector<series>::iterator j = vec.begin(); j != vec.end(); ++j) {
                    series &ser = *j;
                    cout << "        (series for param " << ser.for_param().name() <<
                            "[measured in " << ser.for_param().base_unit().name() << "]" << "): \n";
                    cout << "            series size= " << ser.size() << ", mean=" << ser.mean() << "," <<
                            "variance=" << ser.variance() << ",stdev=" << ser.stdev() << ",last_value=" <<
                            ser.peek() << "\n\n";
                }
                cout << "\n\n";
            }
        }

        static net_info& instance() {
            static net_info the_instance;
            return the_instance;
        }

        /**
          * Returns recorded series of measurements for the specified network parameter.
          * @param net - network
          * @param param - parameter to measure
          */
        const series &param_values(const network &net, const parameter &param) const {
            vector<series> srs = parameter_values_.at(net);
            for (vector<series>::iterator i = srs.begin(); i != srs.end(); ++i) {
                if ((*i).for_param() == param) {
                    return *i;
                }
            }
            throw logic_error("no series found for (net,param)");
        }

        /**
          * Returns the list of networks which currently have any parameter measurements.
          */
        const set<network> network_list() const {
            return networks_;
        }

        /**
          * Returns the list of measured parameters.
          * (It is the same for each network)
          */
        const set<parameter> param_list() const {
            return watched_params_;
        }

        /**
          * This method is periodically called to notify net_info about currently present networks.
          * All information about networks not in the @p nets list is erased.
          * For each network in the @p nets list, measurements of its parameters is performed.
          *
          * @param nets - all detected networks
          */
        void update(set<network> &nets) {
            networks_.clear();
            networks_.insert(nets.begin(), nets.end());

            // remove measurements for no longer existing networks
            for(map< network, vector<series> >::iterator iter = parameter_values_.begin();
                iter != parameter_values_.end(); ) {
                pair< network, vector<series> > val = *iter;
                if (nets.find(val.first) == nets.end()) {
                    // network does not exist
                    parameter_values_.erase(iter++);
                } else {
                    ++iter;
                }
            }

            for (set<network>::iterator i = nets.begin(); i != nets.end(); ++i) {
                const network& net = *i;
                const measurer &meas = MEASURER(net.type());
                vector<series> &svec = parameter_values_[net];

                if (svec.size() > 0) {
                    for (vector<series>::iterator j = svec.begin(); j != svec.end(); ++j) {
                        const parameter &param = (*j).for_param();
                        (*j).push(meas.value(net, param));
                    }
                } else {
                    // no previous measurements - add new series and store values there
                    for (set<parameter>::iterator k = watched_params_.begin(); k != watched_params_.end(); ++k) {
                        parameter param = *k;
                        series ser(param);
                        ser.push(meas.value(net, param));
                        svec.push_back(ser);
                    }
                }
            }
        }
    };
}

#endif // NET_INFO_H
