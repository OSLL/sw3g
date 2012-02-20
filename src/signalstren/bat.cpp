#include "bat.h"
#include <iostream>
#include <fstream>

#include "core/scanner.h"
#include "core/evaluator.h"
#include "core/net_info.h"
#include "impl/scanners.h"
#include "impl/units.h"
#include "impl/eval/single/signal_strength_evaluator.h"

using namespace std;
using namespace fine;
using namespace fine::impl;

void bat::main_sequence() {
    // detecting networks and updating net_info
    set<network> detected;
    for (network_type net_type = FIRST; net_type != LAST; ++net_type) {
        scanner &scan = scanners::instance().get(net_type);
        set<network> detected_for_type = scan.scan();
        if (!detected_for_type.empty()) {
            detected.insert(detected_for_type.begin(), detected_for_type.end());
        }
    }    
    cout << "=============== before update ================" << "\n";
    net_info::instance().dump();
    net_info::instance().update(detected);
    cout << "\n\n=============== after update ================" << "\n";
    net_info::instance().dump();
    cout << endl;

    // evaluate networks and vote for winner
    // currently we have 1 evaluator, so voting is rather simple
    vote for_best_signal(0.08); // relative threshold = 8%
    evaluator *eval = &(signal_strength_evaluator());
    for_best_signal.push_evaluator(eval);
    for_best_signal.push_networks(detected);
    const network &winner = for_best_signal.winner();
    cout << "The winner is: " << winner.id() << " - " << winner.name() << " (type " << winner.type() << ") \n";

    // writing the results to file
    ofstream winner_stream;
    winner_stream.open("aq.txt", ios_base::out);
    winner_stream << network_type_prefixes[winner.type()] << "\n" <<
                     winner.id() << "\n" <<
                     winner.name() << endl;
    winner_stream.close();

    exit(0);
}
