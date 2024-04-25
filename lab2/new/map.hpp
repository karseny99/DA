#pragma once    
#include "RBTree.hpp"
#include <exception>

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

            class TMapIterator {
                private:
                    
                    typename TRBTree<TPair<key_type, mapped_type>>::TIterator current;

                public:

                    TMapIterator(typename TRBTree<TPair<key_type, mapped_type>>::TIterator other) : current(other) {}

                    bool operator==(TMapIterator& other) {
                        return current == other.current;
                    }

                    bool operator!=(TMapIterator& other) {
                        return current != other.current;
                    }

                    const mapped_type operator*() {
                        return *current;
                    }
            };

            TMapIterator begin() const {
                return TMapIterator(rbtree.begin());
            }

            TMapIterator end() const {
                return TMapIterator(rbtree.end());
            }

            bool insert(TPair<key_type, mapped_type> item) {
                return rbtree.insert(item);
            }

            bool erase(const key_type& _key) {
                return rbtree.erase(_key);
            }   

            TMapIterator operator[](const key_type& _key) {
                typename TRBTree<TPair<key_type, mapped_type>>::TIterator value = rbtree.search(_key);
                if(value == rbtree.end()) {
                    rbtree.insert(TPair<key_type, mapped_type>(_key));
                }
                return *value;
            }

            TMapIterator find(const key_type& _key) {
                return TMapIterator(rbtree.search(_key));
            }

            void save_to_file(std::ofstream& os) {
                rbtree.save_to_file(os);
            }

            void load_from_file(std::ifstream& is) {
                rbtree.clear();
                rbtree.load_from_file(is);
            }

    };

};