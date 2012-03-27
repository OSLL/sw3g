#ifndef EVALUATOR_H
#define EVALUATOR_H

#include <vector>
#include <deque>
#include <map>
#include <set>
#include <limits>
#include <cmath>
#include <boost/shared_ptr.hpp>
#include <algorithm>

#include "core/network.h"

using namespace std;
using namespace boost;

namespace fine {
    /**
      * Defines interface for all network evaluators.
      */
    /*abstract*/ class evaluator {
    public:
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

    /**
      * Represents a majority voting mechanism.
      * Multiple evaluators rank multiple networks.
      *
      * The network which has ranked best by maximum number
      * of evaluators is considered the winner.
      */
    class vote {
    private:
        deque<network> networks_;
        vector< shared_ptr<evaluator> > evaluators_;

        double rel_threshold_;
    public:
        /**
          * Creates a voting mechanism with a specified relative
          * threshold (0..1).
          *
          * If @p rel_threshold > 0, the network is considered `better'
          * than the others if its rank is at least rel_threshold*100 %
          * higher. Otherwise it is deemed equivalent to them.
          */
        vote(double rel_threshold = 0):
            rel_threshold_(rel_threshold){
        }

        /**
          * Adds network to vote for.
          * Currently active networks are added to the beginning of the list,
          * and all other networks to the end.
          * @param net - network
          */
        void push_network(const network& net) {
            if (net.is_active()) {
                networks_.push_front(net);
            } else {
                networks_.push_back(net);
            }
        }


        /**
          * Adds networks to vote for.
          * @param nets - network list
          */
        void push_networks(const set<network> &nets) {
            set<network> active_nets;
            set<network> passive_nets;

            for (set<network>::iterator i = nets.begin(); i != nets.end(); ++i) {
                if (i->is_active()) {
                    active_nets.insert(*i);
                } else {
                    passive_nets.insert(*i);
                }
            }

            networks_.insert(networks_.begin(), active_nets.begin(), active_nets.end());
            networks_.insert(networks_.end(), passive_nets.begin(), passive_nets.end());
        }

        /**
          * Adds evaluator to rank networks.
          * @param eval - evaluator
          */
        void push_evaluator(shared_ptr<evaluator> eval) {
            evaluators_.push_back(eval);
        }

        /**
          * Determines whether the election can run
          * (there must be at leas 1 evaluator and at least 1 network
          * to run election.)
          */
        bool can_run() {
            return evaluators_.size() > 0 && networks_.size() > 0;
        }

        /**
          * Determines the election winner.
          *
          * @return the network which was considered best by a maximum
          * number of evaluators.
          */
        const network &winner() {
            cout << "determining winner..." << endl;

            map<network, int> best_count;
            // Let each evaluator rank each network.
            // Count the times each network is considered best by some evaluator.
            for (vector< shared_ptr<evaluator> >::iterator i = evaluators_.begin(); i != evaluators_.end(); ++i) {
                shared_ptr<evaluator> eval = *i;
                double max_rank = eval->max_rank();
                double min_rank = eval->min_rank();
                double abs_threshold = rel_threshold_ * (max_rank - min_rank);

                cout << "abs_threshold for evaluator=" << abs_threshold << endl;

                network &best_net = *(networks_.begin());
                double best_net_rank = -numeric_limits<double>::max();
                for (deque<network>::iterator j = networks_.begin(); j != networks_.end(); ++j) {
                    network& net = *j;
                    double rank = eval->rank(net);
                    cout << "ranked net " << net.name() << "... rank=" << rank << endl;

                    if (rank - best_net_rank > abs_threshold) {
                        cout << "    " << net.name() <<  " - is better than all previous" << endl;
                        best_net = net;
                        best_net_rank = rank;
                    }
                }
                cout << "found best net, it is: " << best_net.name() << endl;
                best_count[best_net] = best_count[best_net] + 1;
            }

            cout << "determining overall..." << endl;

            network& best_net_overall = *(networks_.begin());
            int best_net_count = 0;
            for (map<network, int>::iterator i = best_count.begin(); i != best_count.end(); ++i) {
                pair<network, int> rank_pair = *i;
                network& net = rank_pair.first;
                int count = rank_pair.second;
                if (count > best_net_count) {
                    best_net_overall = net;
                    best_net_count = count;
                    cout << "found best count - " << count << " - for " << net.name() << endl;
                }
            }

            cout << "found overall winner, it is: " << best_net_overall.name() << endl;

            return best_net_overall;
        }
    };
}

#endif // EVALUATOR_H
