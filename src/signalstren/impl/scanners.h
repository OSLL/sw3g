#ifndef SCANNERS_H
#define SCANNERS_H

#include <map>
#include <boost/shared_ptr.hpp>

#include "core/scanner.h"
#include "impl/umts/umts_scanner.h"
#include "impl/wlan/wlan_scanner.h"

namespace fine {
    namespace impl {
        using namespace std;
        using namespace boost;

        class scanners: public scanner_instance {
        private:
            map< network_type, shared_ptr<scanner> > scanners_;

            scanners() {
                scanners_.insert(make_pair(UMTS, shared_ptr<scanner>(new umts_scanner())));
                scanners_.insert(make_pair(WLAN, shared_ptr<scanner>(new wlan_scanner())));
            }

            scanners(const scanners&) {
            }

            scanners& operator=(scanners&) {
            }
        public:
            scanner& get(network_type type) {
                pair< network_type, shared_ptr<scanner> > found = *(scanners_.find(type));
                return *(found.second);
            }

            static scanners& instance() {
                static scanners the_instance;
                return the_instance;
            }
        };
    }
}

#endif // SCANNERS_H
