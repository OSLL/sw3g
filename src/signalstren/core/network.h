#ifndef NETWORK_H
#define NETWORK_H

/**
  * @file scanner.h
  */

#include <string>
#include <boost/noncopyable.hpp>

using namespace std;

namespace fine {
    /**
      * Network type
      */
    enum network_type {
        FIRST,
        /**
          * Wireless LAN (802.11x)
          */
        WLAN = FIRST,
        /**
          * Any kind of 3G network, including HSDPA/HSUPA
          */
        UMTS,
        LAST
    };

    /**
      * Prefixes for switch scripts.
      * Indexed by network_type.
      */
    string network_type_prefixes[] = {"wlan", "3g"};

    /**
      * Operator++ to support enumeration over supported network types.
      */
    network_type& operator++(network_type& type) {
        type = static_cast<network_type>(type + 1);
        return type;
    }

    /**
      * Network information key: unique ID (e.g. "00:1E:58:B8:AB:A3"), name (e.g. "osll"),
      * type (e.g., WLAN, 3G), and a number of arbitrary attributes.
      *
      * Network information key is immutable.
      */
    class network {
    private:
        std::string id_;
        std::string name_;
        network_type type_;
    public:
        /**
          * Creates a network information entry with specified
          * name and type.
          */
        network(const std::string &id, const std::string &name, const network_type type):
            id_(id), name_(name), type_(type) {
        }

        /**
          * Duplicates an existing network information entry.
          */
        network(const network &rhs):
            id_(rhs.id_), name_(rhs.name_), type_(rhs.type_) {
        }

        bool operator==(const network& rhs) const {
            return rhs.type_ == type_ && rhs.id_ == id_;
        }

        bool operator<(const network& rhs) const {
            return rhs.type_ < type_ || (rhs.type_ == type_ && rhs.id_ < id_);
        }

        /**
          * Returns the network identifier, unique among networks of the same type.
          * e.g. MAC address for WLAN, PLMN for 3G, ...
          */
        const std::string &id() const {
            return id_;
        }

        /**
          * Returns user-friendly name of the network, e.g. "osll".
          */
        const std::string &name() const {
            return name_;
        }

        /**
          * Returns the network type.
          */
        network_type type() const {
            return type_;
        }
    };
}

#endif // NETWORK_H
