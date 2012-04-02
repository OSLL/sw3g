#ifndef REGISTRY_H
#define REGISTRY_H

#include <map>
#include <set>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <typeinfo>
#include <iterator>
#include <cstddef>

#include "core/network.h"

using namespace std;
using namespace boost;

namespace fine {
    /**
      * Defines a registry of objects.
      * There may be only one object per network_type.
      */
    template<typename K, typename T>
    class registry: boost::noncopyable {
    private:
        typedef map< K, shared_ptr<T> > map_type;

        map_type objects_;
    public:
        typedef registry<K, T> self;
        typedef typename map_type::const_iterator map_iterator;

        /**
          * Iterator through the values stored in the registry.
          */
        class value_iterator: public std::iterator<std::forward_iterator_tag, T> {
        private:
            map_iterator iter_;
        public:
            typedef std::forward_iterator_tag iterator_category;
            typedef T value_type;
            typedef std::ptrdiff_t difference_type;
            typedef T& reference;
            typedef T* pointer;

            value_iterator(map_iterator iter):
                iter_(iter) {
            }

            value_iterator(const value_iterator &rhs):
                iter_(rhs.iter_) {
            }

            reference operator*() const {
                return *(iter_->second);
            }

            pointer operator->() const {
                return iter_->second;
            }

            value_iterator &operator++() {
                ++iter_;
                return *this;
            }

            value_iterator operator++(int) {
                value_iterator tmp(*this);
                ++*this;
                return tmp;
            }

            void swap(value_iterator &other) {
                std::swap(iter_, other.iter_);
            }

            value_iterator &operator= (value_iterator other) {
                swap(other);
                return *this;
            }

            bool operator== (value_iterator &other) {
                return iter_ == other.iter_;
            }

            bool operator!= (value_iterator &other) {
                return !(*this == other);
            }
        };

        /**
          * Associates the specified copy-constructible object
          * with a key.
          * If association already exists, the operation silently fails.
          *
          * @param type - key
          * @param object - pointer to the object
          */
        void put(K type, T* object) {
            if (!is_registered(type)) {
                // does not yet exist, add item to the map
                objects_.insert(make_pair(type, shared_ptr<T>(object)));
            }
        }

        /**
          * Returns the object corresponding to the network_type.
          * @param type - network type
          * @return object for network of type @p type
          */
        T& get(K type) {
            pair< K, shared_ptr<T> > found = *(objects_.find(type));
            return *(found.second);
        }

        /**
          * Tells whether an object is registered for the specified
          * network type.
          * @param type - key
          * @return true if registration exists
          */
        bool is_registered(K type) {
            return objects_.count(type) > 0;
        }

        /**
          * Returns a shared instance of the registry.
          */
        static self& instance() {
            static self the_instance;
            return the_instance;
        }

        /**
          * Returns a constant iterator through the <K,ptr T>-pairs
          * stored in the registry pointing at the beginning of
          * the registry. (K = key, T = value)
          */
        map_iterator cbegin() {
            return objects_.begin();
        }

        /**
          * Returns a constant iterator through the <K,ptr to T>-pairs
          * stored in the registry pointing past the end of
          * the registry. (K = key, T = value)
          */
        map_iterator cend() {
            return objects_.end();
        }

        /**
          * Returns a constant iterator through the values (T's)
          * stored in the registry pointing at the beginning of
          * the registry.
          */
        value_iterator vcbegin() {
            return value_iterator(cbegin());
        }

        /**
          * Returns a constant iterator through the values (T's)
          * stored in the registry pointing past the end of
          * the registry.
          */
        value_iterator vcend() {
            return value_iterator(cend());
        }
    };

    /**
      * Defines a global list of singleton objects subclassing
      * the same type, T.
      */
    template<typename T>
    class global_list: public boost::noncopyable {
    private:
        vector< shared_ptr<T> > objects_;
        set<string> type_ids_;
    public:
        /**
          * Adds a specified object to the list.
          * If object of this type already exists in the list, it is
          * not added.
          *
          * @param object - pointer to the object
          */
        void put(T* object) {
            if (!is_registered(object)) {
                // object with this type does not yet exist, add new item
                objects_.push_back(shared_ptr<T>(object));
                type_ids_.insert(typeid(object).name());
            }
        }

        /**
          * @return The size of the list.
          */
        size_t size() {
            return objects_.size();
        }

        /**
          * Returns the index'th object (counting from 0)
          */
        shared_ptr<T> operator[](int index) {
            return objects_[index];
        }

        /**
          * Tells whether an object is already in the list
          * @param object - pointer to object
          * @return true if object of this type has already been registered
          */
        bool is_registered(T* object) {
            return type_ids_.find(typeid(object).name()) != type_ids_.end();
        }

        /**
          * Returns a shared instance of the global list.
          */
        static global_list<T>& instance() {
            static global_list<T> the_instance;
            return the_instance;
        }
    };
}

#endif // REGISTRY_H
