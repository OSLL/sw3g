#ifndef WLAN_SCANNER_H
#define WLAN_SCANNER_H

#include <set>
#include <string>
#include <iostream>
#include <istream>
#include <sstream>
#include <typeinfo>

#include "core/scanner.h"
#include "util.h"

#include "core/registries.h"

#include "wlan_measurer.h"

using namespace std;

namespace fine {
    namespace impl {
        /**
          * Scanner for WLAN networks
          */
        class wlan_scanner: public scanner {
        public:
            set<network> scan() {
                // invalidate the measurement cache
                wlan_measurer &wm = dynamic_cast<wlan_measurer&>(MEASURER(WLAN));
                wm.cache_invalid_ = true;

                set<network> result;

                string active_net_id = exec("script/wlan_active_net.sh");
                string net_data = exec("script/wlan_scan.sh");
                istringstream str_net_data(net_data);

                while (str_net_data) {
                    string network_id;
                    string network_name;
                    str_net_data >> network_id;
                    if (str_net_data)
                        str_net_data >> network_name;

                    if (network_id.length() > 0) {
                        cout << network_id << "/" << network_name << endl;
                        result.insert(network(network_id, network_name, WLAN,
                            (network_id == active_net_id)));
                    }
                }
                return result;
            }
        };
    }
}

#endif // WLAN_SCANNER_H
