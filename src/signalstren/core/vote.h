#ifndef VOTE_H
#define VOTE_H

#include <boost/shared_ptr.hpp>
#include <vector>
#include <map>
#include <set>
#include <limits>
#include <cmath>
#include <algorithm>
#include <deque>

#include "network.h"
#include "net_info.h"

using namespace std;
using namespace boost;

namespace fine {
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
        static const size_t MINIMAL_NETWORK_AGE;

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
          * Adds a network to vote for.
          *
          * Network must have age >= vote::MINIMAL_NETWORK_AGE; otherwise,
          * the call will silently fail.
          *
          * Currently active networks are added to the beginning of the list,
          * and detected networks to the end. Vanished networks are not added
          * at all.
          *
          * @param net - network
          */
        void push_network(const network& net) {
            if (net.is_vanished())
                return;

            if (net_info::instance().age(net) < MINIMAL_NETWORK_AGE)
                return;

            if (net.is_connected()) {
                networks_.push_front(net);
            } else {
                networks_.push_back(net);
            }
        }


        /**
          * Adds networks to vote for.
          * See docs for push_network(const network&) function for details
          * on which networks from the list will actually be added.
          *
          * @param nets - network list
          */
        void push_networks(const set<network> &nets) {
            for (set<network>::iterator i = nets.begin(); i != nets.end(); ++i) {
                push_network(*i);
            }
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
            cout << "================= ranking ===================" << endl;

            map<network, int> best_count;
            // Let each evaluator rank each network.
            // Count the times each network is considered best by some evaluator.
            for (vector< shared_ptr<evaluator> >::iterator i = evaluators_.begin(); i != evaluators_.end(); ++i) {
                shared_ptr<evaluator> eval = *i;
                double max_rank = eval->max_rank();
                double min_rank = eval->min_rank();
                double abs_threshold = rel_threshold_ * (max_rank - min_rank);

                string eval_title = "[" + eval->name() + "]";

                network &best_net = *(networks_.begin());
                double best_net_rank = -numeric_limits<double>::max();
                for (deque<network>::iterator j = networks_.begin(); j != networks_.end(); ++j) {
                    network& net = *j;
                    double rank = eval->rank(net);
                    cout << eval_title << " " << net.name() << " rank=" << rank << endl;

                    if (rank - best_net_rank > abs_threshold) {
                        best_net = net;
                        best_net_rank = rank;
                    }
                }
                cout << eval_title << " found best net, it is: " << best_net.name() << endl;
                best_count[best_net] = best_count[best_net] + 1;
            }

            cout << "================== voting ===================" << "\n";
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

#endif // VOTE_H