#pragma once
#include <iostream>
#include <RBTree.hpp>


namespace mmap {

    template<class T1, class T2>
    class pair {
        T1 first;
        T2 second;
    };

    template <class key_type, class value_type>
    class map {
        private:
            
            std::shared_ptr<RBTree<pair<key_type, std::shared_ptr<value_type>>>> tree;

        public:

            bool insert() {

            }

    };

};