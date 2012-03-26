#include "bat.h"
#include <iostream>
#include <fstream>

#include "core/scanner.h"
#include "core/evaluator.h"
#include "core/network.h"
#include "core/net_info.h"

#include "core/registries.h"

#include "impl/config.h"

using namespace std;
using namespace fine;
using namespace fine::impl;

void bat::main_sequence() {
    // detecting networks and updating net_info
    set<network> detected;
    map< network_type, set<network> > detected_by_type;
    for (network_type net_type = FIRST; net_type != LAST; ++net_type) {        
        if (!scanners::instance().is_registered(net_type)) {
            cout << "WARNING: network type " << net_type << " (" << network_type_prefixes[net_type] <<
                    ") not registered" << "\n";
        }

        scanner &scan = scanners::instance().get(net_type);
        set<network> detected_for_type = scan.scan();
        if (!detected_for_type.empty()) {
            detected.insert(detected_for_type.begin(), detected_for_type.end());
        }

        detected_by_type[net_type] = detected_for_type;
    }    
    cout << "=============== before update ================" << "\n";
    net_info::instance().dump();
    net_info::instance().update(detected);
    cout << "\n\n=============== after update ================" << "\n";
    net_info::instance().dump();
    cout << endl;

    //// Evaluation:
    ofstream winner_stream;
    winner_stream.open("aq.txt", ios_base::out);

    const double REL_THRESHOLD = 0.08; // relative threshold = 8%

    // find overall winner
    vote overall_best(REL_THRESHOLD);
    for (int i = 0; i < evaluators::instance().size(); ++i) {
        overall_best.push_evaluator(evaluators::instance()[i]);
    }
    overall_best.push_networks(detected);
    if (overall_best.can_run()) {
        const network &winner = overall_best.winner();
        cout << "The overall winner is: " << winner.id() << " - " << winner.name() << " (type " << network_type_prefixes[winner.type()] << ") \n";

        // writing the results to file
        winner_stream << "best=" <<
                         network_type_prefixes[winner.type()] << " " <<
                         winner.id() << " " <<
                         winner.name() << endl;
    } else {
        winner_stream << "best=none" << endl;
        cout << "No networks detected :-(" << endl;
    }

    // find winner for each network type
    for (network_type net_type = FIRST; net_type != LAST; ++net_type) {
        vote net_best(REL_THRESHOLD);
        for (int i = 0; i < evaluators::instance().size(); ++i) {
            net_best.push_evaluator(evaluators::instance()[i]);
        }
        net_best.push_networks(detected_by_type[net_type]);
        if (net_best.can_run()) {
            const network &winner = net_best.winner();
            cout << "The winner for type " << network_type_prefixes[net_type] << " is: "
                 << winner.id() << " - " << winner.name() << " (type "
                 << network_type_prefixes[winner.type()] << ") \n";

            winner_stream << "best[" << network_type_prefixes[net_type] << "]=" <<
                             winner.id() << " " <<
                             winner.name() << endl;
        } else {
            cout << "No networks detected :-(" << endl;
            winner_stream << "best[" << network_type_prefixes[net_type] << "]=none" << endl;
        }
    }
    winner_stream.close();

    exit(0);
}
