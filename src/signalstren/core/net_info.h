#ifndef NET_INFO_H
#define NET_INFO_H

#include <set>
#include <map>
#include <vector>
#include <algorithm>
#include <boost/noncopyable.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <iostream>
#include <fstream>
#include <ctime>

#include "core/network.h"
#include "core/parameter.h"
#include "core/series.h"
#include "core/registries.h"

#include "core/persist/net_info.pb.h"

namespace fine {
    using namespace std;
    using namespace google::protobuf;

    /**
      * Stores Network Information: which networks are available,
      * what parameters are measured for each, what parameter
      * values are.
      */
    class net_info: public boost::noncopyable {
    private:
        /**
          * Persistent storage filename
          */        
        const static string NETINFO_FILENAME;

        /**
          * Netinfo initialization time
          */
        posix_time::ptime init_time_;

        /**
          * List of parameter measurement series for each network
          */
        map< network,vector<series> > parameter_values_;

        /**
          * List of measured parameters
          */
        set<parameter> watched_params_;

        /**
          * List of networks which we know about
          */
        set<network> networks_;

        /**
          * Network ages
          */
        map<network, size_t> network_ages_;

        void initialize_params() {
            watched_params_.insert(params::instance().vcbegin(), params::instance().vcend());
        }

        void initialize_measurements(const mnetinfo &m_info) {
            for (RepeatedPtrField<mnetwork>::const_iterator i = m_info.all_networks().begin();
                 i != m_info.all_networks().end(); ++i) {
                const mnetwork &m_net = *i;
                network net(m_net.id(), m_net.name(), (network_type) m_net.type(),
                            (network_status) m_net.status(), m_net.vanishing_timer());
                networks_.insert(net);
                network_ages_[net] = m_net.time_of_existence();

                vector<series> series_for_net;
                for (RepeatedPtrField<mseries>::const_iterator j = m_net.all_series().begin();
                     j != m_net.all_series().end(); ++j) {
                    const mseries &m_ser = *j;

                    series ser(PARAM(m_ser.parameter()));
                    ser.count_ = m_ser.size();
                    ser.current_ = m_ser.last_value();
                    ser.new_m_ = m_ser.new_m();
                    ser.old_m_ = m_ser.old_m();
                    ser.new_s_ = m_ser.new_s();
                    ser.old_s_ = m_ser.old_s();

                    series_for_net.push_back(ser);
                }

                parameter_values_[net] = series_for_net;
            }
        }

        net_info():
        init_time_(posix_time::microsec_clock::universal_time()) {
            initialize_params();

            filebuf buf;
            buf.open(NETINFO_FILENAME.c_str(), ios_base::in);
            // check if we found the file
            if (!buf.is_open())
                return;

            // read all file data into array
            size_t size = buf.pubseekoff(0, ios_base::end, ios_base::in);
            buf.pubseekpos(0, ios_base::in);
            char* data_buffer = new char[size];
            buf.sgetn(data_buffer, size);

            mnetinfo m_info;
            // check if the data represents a correct netinfo state
            if (!m_info.ParseFromArray(data_buffer, size))
                return;

            // get last call time from netinfo storage
            posix_time::ptime last_call = posix_time::from_iso_string(m_info.last_call_time());

            cout << "=== Last program run: " << last_call << " ===" << endl;

            if ((init_time_ - last_call).total_seconds() > CACHE_LIFETIME) {
                // if we were called after NETINFO_CACHE_LIFETIME seconds,
                // discard all information from the persistent storage
                // and start anew.
                cout << "Discarded all previous measurements (if any)." << endl;
            } else {
                initialize_measurements(m_info);
            }

            delete[] data_buffer;
        }

        void erase_net(map< network, vector<series> >::iterator &iter) {
            map<network, size_t>::iterator found = network_ages_.find(iter->first);
            if (found != network_ages_.end()) {
                network_ages_.erase(found);
            }

            parameter_values_.erase(iter++);
        }

        /**
          * remove measurements for no longer existing networks
          * @param nets - detected networks
          */
        void prune_network_list(set<network> &nets) {
            // increment ages for all detected networks
            for(set<network>::iterator iter = nets.begin(); iter != nets.end(); ++iter) {
                network_ages_[*iter]++;
            }

            for(map< network, vector<series> >::iterator iter = parameter_values_.begin();
                iter != parameter_values_.end();) {
                const network &net = iter->first;

                if (nets.find(net) != nets.end()) {
                    // network is in current scan as well in the previous
                    // ones
                    ++iter;
                    continue;
                }

                // found network present in previous scan(s), but absent
                // in the current
                switch(net.status()) {
                case CONNECTED:
                    // if network which we were connected to does not
                    // appear in the scan, it is deleted immediately,
                    // because a disconnection (likely) happened.
                    erase_net(iter);
                    break;
                case DETECTED:
                case VANISHED:
                    const network &vanished = net.vanish();
                    if (vanished.vanishing_counter() <= VANISHING_TIMEOUT) {
                        // network was detected in some previous scan,
                        // but is absent in current
                        networks_.insert(vanished);
                        vector<series> all_series = parameter_values_[net];
                        parameter_values_.erase(parameter_values_.find(net));
                        parameter_values_[vanished] = all_series;

                        // important: network age is not incremented
                        // for vanished networks

                        ++iter;
                    } else {
                        // network vanished for too long, removing from the list
                        erase_net(iter);
                    }
                    break;
                }
            }
        }

        /**
          * read and record measurements for all detected networks
          * @param nets - detected networks
          */
        void read_measurements(set<network> &nets) {
            for (set<network>::iterator i = nets.begin(); i != nets.end(); ++i) {
                const network& net = *i;
                const measurer &meas = MEASURER(net.type());
                vector<series> &svec = parameter_values_[net];

                if (svec.size() > 0) {
                    for (vector<series>::iterator j = svec.begin(); j != svec.end(); ++j) {
                        const parameter &param = (*j).for_param();

                        if (meas.has_measurement(net, param)) {
                            (*j).push(meas.value(net, param));
                        }
                    }
                } else {
                    // no previous measurements - add new series and store values there
                    for (set<parameter>::iterator k = watched_params_.begin(); k != watched_params_.end(); ++k) {
                        parameter param = *k;
                        series ser(param);
                        if (meas.has_measurement(net, param)) {
                            ser.push(meas.value(net, param));
                        }

                        svec.push_back(ser);
                    }
                }
            }
        }

    public:
        /**
          * Network Info cache lifetime
          */
        const static int CACHE_LIFETIME;

        /**
          * Maximum number of calls to signalstren during which the network
          * may remain in VANISHING state. After this number of calls to
          * signalstren is made, network is deleted from the list.
          */
        const static size_t VANISHING_TIMEOUT;

        void dump() {
            for (map< network, vector<series> >::iterator i = parameter_values_.begin(); i != parameter_values_.end(); ++i) {
                const network &net = i->first;
                cout << net.id() << " " << net.name() << " " << net.type() << ",age=" <<
                     age(net) << ",vanishing_counter=" << net.vanishing_counter() << endl;
                vector<series> &vec = i->second;
                for (vector<series>::iterator j = vec.begin(); j != vec.end(); ++j) {
                    series &ser = *j;
                    cout << "  " << ser.for_param().name() <<
                            "[" << ser.for_param().base_unit().name() << "]" << ": " <<
                            "size=" << ser.size() << ",mean=" << ser.mean() << "," <<
                            "variance=" << ser.variance() << ",stdev=" << ser.stdev() <<
                            ",last_value=" << ser.peek() << endl;
                }
            }
        }

        ~net_info() {
            mnetinfo m_info;

            for (map< network, vector<series> >::iterator i = parameter_values_.begin();
                 i != parameter_values_.end(); ++i) {
                mnetwork *m_net = m_info.add_all_networks();

                const network &net = i->first;
                m_net->set_id(net.id());
                m_net->set_status((mnetwork_estatus) net.status());
                m_net->set_name(net.name());
                m_net->set_type((mnetwork_etype) net.type());
                m_net->set_vanishing_timer(net.vanishing_counter());
                m_net->set_time_of_existence(age(net));

                vector<series> &ser = i->second;

                for (vector<series>::iterator j = ser.begin(); j != ser.end(); ++j) {
                    mseries *m_ser = m_net->add_all_series();

                    m_ser->set_last_value(j->peek());
                    m_ser->set_size(j->size());
                    m_ser->set_new_m(j->new_m_);
                    m_ser->set_old_m(j->old_m_);
                    m_ser->set_new_s(j->new_s_);
                    m_ser->set_old_s(j->old_s_);

                    m_ser->set_parameter(j->for_param().name());
                }
            }

            m_info.set_last_call_time(posix_time::to_iso_string(init_time_));

            ofstream file_stream;
            file_stream.open(NETINFO_FILENAME.c_str());
            string target;
            m_info.SerializeToString(&target);

            file_stream << target;
            file_stream.close();
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
          * Returns time of existence for the network (number of calls to signalstren
          * during which the network was detected in scans).
          *
          * @param net - network
          */
        size_t age(const network &net) {
            return network_ages_[net];
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

            prune_network_list(nets);
            read_measurements(nets);
        }
    };

    const string net_info::NETINFO_FILENAME("/tmp/.fine-netinfo");
}

#endif // NET_INFO_H
