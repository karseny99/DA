#pragma once
#include <iostream>
#include "RBTree.hpp"


namespace NMap {

    template<class T1, class T2>
    class TPair {
        public:

            typedef T1 first_type;
            typedef T2 second_type;
            T1 first;
            T2 second;
            TPair() = default;
            TPair(const T1& fst, const T2& scd) : first(fst), second(scd) {}

            TPair(T1 _first) : first(_first) {
                second = T2();
            }
    };

    template <typename key, typename data>
    class TMap {
        public:
            typedef key key_type;
            typedef data mapped_type;
        private:

            TRBTree<TPair<key_type, mapped_type>> rbtree;

        public:

            bool insert(TPair<key_type, mapped_type> item) {
                return rbtree.insert(item);
            }

            bool erase(key_type _key) {
                return rbtree.erase(_key);
            }   

            mapped_type& operator[](key_type _key) {
                return rbtree.search(_key);
            }

            void save_to_file(std::ofstream& os) {
                rbtree.save_to_file(os);
            }

            void load_from_file(std::ifstream& is) {
                rbtree.load_from_file(is);
            }

    };

};