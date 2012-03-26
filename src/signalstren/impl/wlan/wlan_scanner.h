#ifndef WLAN_SCANNER_H
#define WLAN_SCANNER_H

#include <set>
#include <string>
#include <iostream>
#include <istream>
#include <sstream>

#include "core/scanner.h"
#include "util.h"

using namespace std;

namespace fine {
    namespace impl {
        /**
          * Scanner for WLAN networks
          */
        class wlan_scanner: public scanner {
        public:
            set<network> scan() {
                set<network> result;

                string net_data = exec("script/getnetdata.sh");
                istringstream str_net_data(net_data);

                while (str_net_data) {
                    string network_id;
                    string network_name;
                    str_net_data >> network_name;
                    if (str_net_data)
                        str_net_data >> network_id;

                    if (network_id.length() > 0) {
                        cout << network_id << "/" << network_name << endl;
                        result.insert(network(network_id, network_name, WLAN));
                    }
                }
                return result;
            }
        };
    }
}

#endif // WLAN_SCANNER_H
