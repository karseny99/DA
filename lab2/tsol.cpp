#include <memory>
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdint>

/*
Red Black Tree:
    Insertion:
        dad is red:
            uncle is black => rotate
            uncle is red => set parent's and uncle's color to grandparents => recursively get next fix up
    
    Deletion:
        first step similar to default binary search tree: 
            if two children, get min from right subtree
            else replace deleted with its child(if needed) 
        
        red node => doesn't change the black height => don't care
        brother is black:
            parent is red => set parent black, brother to red => black height fixed 
            parent is black => if parent == root => case above will help, else recursively get next fix up

        brother is red:
            rotate from brother => getting above case (brother became black)
*/

namespace NMap {

    enum colors {
        RED, 
        BLACK
    };

    const std::string BNODE = "BNODE";
    const std::string ENDN = "END";
    const std::string red = "\033[0;31m";
    const std::string reset = "\033[0m";

    const int LEFT = 0;
    const int RIGHT = 1;

    template <class TPair>
    class TRBTree {
        private:
       
            class TNode {
                private:
                    const int SPACES_COUNT = 2;

                public:
                    TPair item;
                    colors color = BLACK;
                    std::shared_ptr<TNode> child[2];

                    TNode(colors _color, \
                        TPair _item, \
                        std::shared_ptr<TNode> _left, \
                        std::shared_ptr<TNode> _right) : 
                            color(_color), 
                            item(_item) {
                                child[LEFT] = _left;
                                child[RIGHT] = _right;
                            }
                    
                    ~TNode() = default;

                    bool value_exists(TPair _item) {
                        return (item.first == _item.first) or 
                            (child[LEFT] != nullptr and child[LEFT]->item.first == _item.first) or 
                            (child[RIGHT] != nullptr and child[RIGHT]->item.first == _item.first);
                    }   
                    
                    void print(int space = 0) {
                        space += 2;

                        if(child[RIGHT])
                            child[RIGHT]->print(space);
                        for(int i = SPACES_COUNT; i < space; ++i) {
                            std::cout << ' ' ;
                        }
                        if(color == RED)
                            std::cout << red << item.first << reset << std::endl;
                        else 
                            std::cout << item.first << std::endl;
                            
                        if(child[LEFT])
                            child[LEFT]->print(space);
                    }

                    bool has_no_children() {
                        return (child[LEFT] == nullptr) and (child[RIGHT] == nullptr);
                    }

                    bool has_one_child() {
                        return  (child[LEFT] != nullptr) ^ (child[RIGHT] != nullptr);
                    }

                    bool has_two_children() {
                        return (child[LEFT] != nullptr) and (child[RIGHT] != nullptr);
                    }

                    void write_data_to_file(std::ostream& os) const {
                        os.write(BNODE.c_str(), BNODE.size()); // 5 bytes 

                        // first value(Key)
                        size_t sizeOfKey = item.first.size();
                        os.write(reinterpret_cast<const char*>(&sizeOfKey), sizeof(size_t));  
                        os.write(item.first.c_str(), sizeOfKey); 

                        // second value(Value)
                        uint64_t second_value = item.second;
                        os.write(reinterpret_cast<const char*>((&second_value)), sizeof(second_value));  

                        // node's color
                        if(color == BLACK) 
                            os.put('1');
                        else 
                            os.put('0');
                        if(child[LEFT]) child[LEFT]->write_data_to_file(os);
                        else os.write(ENDN.c_str(), ENDN.size()); // 5 bytes
                        if(child[RIGHT]) child[RIGHT]->write_data_to_file(os);
                        else os.write(ENDN.c_str(), ENDN.size()); // 5 bytes
                    }

            };


            bool is_red(std::shared_ptr<TNode> TNode) const {
                return (TNode != nullptr and TNode->color == RED);
            }

            bool is_black(std::shared_ptr<TNode> TNode) const {
                return (TNode == nullptr or TNode->color == BLACK);
            }
        
            std::shared_ptr<TNode> rotate(std::shared_ptr<TNode> currentRoot, bool direction) { // left / right
                std::shared_ptr<TNode> newRoot = currentRoot->child[!direction];

                currentRoot->child[!direction] = newRoot->child[direction];
                newRoot->child[direction] = currentRoot;

                newRoot->color = currentRoot->color;
                currentRoot->color = RED;

                return newRoot;
            }    

            std::shared_ptr<TNode> doubleRotate(std::shared_ptr<TNode> currentRoot, bool direction) {
                currentRoot->child[!direction] = rotate(currentRoot->child[!direction], !direction);
                return rotate(currentRoot, direction);
            }

            // sets opposite color to root and its children
            void change_colors(std::shared_ptr<TNode> TNode) { 
                TNode->color = static_cast<colors>(!TNode->color);
                if(TNode->child[LEFT])
                    TNode->child[LEFT]->color = static_cast<colors>(!TNode->child[LEFT]->color);
                if(TNode->child[RIGHT])
                    TNode->child[RIGHT]->color = static_cast<colors>(!TNode->child[RIGHT]->color);
            }

            std::shared_ptr<TNode> check_add_correctness(std::shared_ptr<TNode> localRoot, bool direction) {
                
                if(is_red(localRoot->child[direction])) {
                    
                    // parent is red, uncle is red -> simply switch colors
                    if(is_red(localRoot->child[!direction])) {
                        if(is_red(localRoot->child[direction]->child[direction]) or is_red(localRoot->child[direction]->child[!direction])) {
                            // std::cout << "->Uncle is red, changing colors" << std::endl;
                            change_colors(localRoot); 
                        }

                    // parent is red, uncle is black -> need rotate
                    } else {
                        // order is right
                        if(is_red(localRoot->child[direction]->child[direction])) {
                            // std::cout << "->Uncle is black, rotating" << std::endl;

                            localRoot = rotate(localRoot, !direction);
                        // order is wrong, need to rotate twice
                        } else if(is_red(localRoot->child[direction]->child[!direction])) {
                            // std::cout << "->Uncle is black, rotating" << std::endl;

                            localRoot = doubleRotate(localRoot, !direction);
                        }
                    }
                    
                } 

                return localRoot;
            } 

            std::shared_ptr<TNode> _insert(std::shared_ptr<TNode> currentTNode, const TPair& item, bool& is_found) {

                if(currentTNode == nullptr) 
                    return std::make_shared<TNode>(RED, item, nullptr, nullptr);

                if(currentTNode->item.first == item.first) {
                    is_found = true;
                    return currentTNode;
                }

                bool direction = (item.first > currentTNode->item.first);
                currentTNode->child[direction] = _insert(currentTNode->child[direction], item, is_found);

                return check_add_correctness(currentTNode, direction);
            }

            std::shared_ptr<TNode> get_minimum(std::shared_ptr<TNode> currentRoot) {
                auto mover = currentRoot;
                while(mover->child[LEFT] != nullptr) {
                    mover = mover->child[LEFT];
                }

                return mover;
            }

            std::shared_ptr<TNode> check_erase_correctness(std::shared_ptr<TNode> currentTNode, bool direction, bool& needBalance) {
                std::shared_ptr<TNode> parent = currentTNode;
                std::shared_ptr<TNode> brother = currentTNode->child[!direction];

                if(is_red(brother)) {
                    currentTNode = rotate(currentTNode, direction);
                    brother = parent->child[!direction];
                }

                if(brother) {

                    if(is_black(brother->child[LEFT]) and is_black(brother->child[RIGHT])) {

                        if(is_red(parent)) needBalance = false;

                        parent->color = BLACK;
                        brother->color = RED;

                    } else {
                        colors parentColor = parent->color;
                        bool isRedBrotherReduction = (currentTNode != parent);

                        if(is_red(brother->child[!direction])) {
                            parent = rotate(parent, direction);
                        } else {
                            parent = doubleRotate(parent, direction);
                        }

                        parent->color = parentColor;
                        parent->child[LEFT]->color = BLACK;
                        parent->child[RIGHT]->color = BLACK;

                        if(isRedBrotherReduction) {
                            currentTNode->child[direction] = parent;
                        } else {
                            currentTNode = parent;
                        }

                        needBalance = false;
                    }
                }
                return currentTNode;
            }

        private:

            std::shared_ptr<TNode> root = nullptr;
        
        private:
         
            std::shared_ptr<TNode> _erase(std::shared_ptr<TNode> currentTNode, TPair::first_type key, bool& needBalance, bool& erased) {

                if(currentTNode == nullptr)  {
                    needBalance = false;
                    erased = false;
                    return nullptr;
                }

                if(currentTNode->item.first == key) {

                    // has zero or one child
                    if(currentTNode->child[LEFT] == nullptr or currentTNode->child[RIGHT] == nullptr) {

                        std::shared_ptr<TNode> tmp;

                        if(currentTNode->child[LEFT]) tmp = currentTNode->child[LEFT];
                        if(currentTNode->child[RIGHT]) tmp = currentTNode->child[RIGHT];

                        // If TNode is red, the black height is not changing -> don't need to do anything                
                        if(is_red(currentTNode)) {
                            currentTNode = nullptr;
                            needBalance = false;
                        
                        // if TNode is black and its child is red -> simply replace colors;
                        } else if(is_red(tmp)) {
                            currentTNode = nullptr;
                            tmp->color = BLACK;
                            needBalance = false;
                        }

                        return tmp;

                    // has two children, find minimum in right subtree and copy its value in currentTNode, 
                    // then delete this minimum from right subtree
                    } else {
                        std::shared_ptr<TNode> tmp = get_minimum(currentTNode->child[RIGHT]);
                        // std::cout << tmp->value << std::endl;

                        currentTNode->item = tmp->item;
                        key = tmp->item.first;
                    }
                }

                bool direction = (key >= currentTNode->item.first);
                
                currentTNode->child[direction] = _erase(currentTNode->child[direction], key, needBalance, erased);

                return needBalance ? check_erase_correctness(currentTNode, direction, needBalance) : currentTNode;
            } 

            TPair::second_type& get_lvalue(std::shared_ptr<TNode> currentNode, TPair::first_type key) {
                if(currentNode == nullptr) {
                    throw std::runtime_error("Error: can't reach such element");
                }
                if(key == currentNode->item.first) {
                    return currentNode->item.second;
                }
                bool direction = (key >= currentNode->item.first);
                return get_lvalue(currentNode->child[direction], key);
            }

            std::shared_ptr<TPair> _search(const TPair::first_type& key, std::shared_ptr<TNode> currentTNode) {
                // if tree doesn't have this key => adding
                if(currentTNode == nullptr) {
                    return nullptr;
                }
                if(key == currentTNode->item.first) {
                    return std::make_shared<TPair>(currentTNode->item);
                }
                bool direction = (key >= currentTNode->item.first);
                return _search(key, currentTNode->child[direction]);
            }

        public: 

            TRBTree() = default;

            bool insert(const TPair& item) {
                bool is_found = false;
                root = _insert(root, item, is_found);
                root->color = BLACK;
                if(is_found)
                    return false;
                return true;
            }

            bool erase(const TPair::first_type& key) {
                bool erased = true;
                bool needBalanced = true;
                root = _erase(root, key, needBalanced, erased);
                if(root)
                    root->color = BLACK;
                return erased;
            }

            std::shared_ptr<TPair> search(const TPair::first_type& key) {
                return _search(key, root);
            }

            void save_to_file(std::ofstream& os) {
                if(root)
                    root->write_data_to_file(os);
            }
            
            std::shared_ptr<TNode> load_from_file(std::shared_ptr<TNode> _root, std::ifstream& is) {
                if(is.peek() == 'B') {
                    char buffer[BNODE.size()];

                    // BNODE reading
                    is.read(buffer, (BNODE.size()));
                    std::string stringBuffer = static_cast<std::string>(buffer);

                    // read size of key
                    size_t size;
                    is.read(reinterpret_cast<char *>(&size), sizeof(size_t));
                    
                    // read key
                    char * temp = new char[size + 1];
                    is.read(temp, size);
                    temp[size] = '\0';
                    std::string key = temp;
                    delete[] temp;

                    // read value
                    uint64_t value;
                    is.read(reinterpret_cast<char *>(&value), sizeof(value));
                    
                    // read color
                    colors nodeColor;
                    char tmpColor = is.get();
                    if(tmpColor == '1') 
                        nodeColor = BLACK;
                    else 
                        nodeColor = RED;
                    
                    TPair tmpData;
                    tmpData.first = key;
                    tmpData.second = value;
                    std::shared_ptr<TNode> _root = std::make_shared<TNode>(nodeColor, tmpData, nullptr, nullptr);
                    
                    _root->child[LEFT] = load_from_file(_root, is);
                    _root->child[RIGHT] = load_from_file(_root, is);

                    return _root;
                } else if(is.peek() == 'E') {
                    char buffer[ENDN.size()];
                    is.read(buffer, (ENDN.size()));
                    return nullptr;
                }
            }

            TPair::second_type& operator[](TPair::first_type _key) {
                return get_lvalue(root, _key);
            }

            void load_from_file(std::ifstream& is) {
                root = load_from_file(root, is);
            }

            void clear() {
                if(root)
                    root = nullptr;
            }

            friend void print<TPair>(TRBTree<TPair>* tree);

    };

    template<class TPair>
    void print(TRBTree<TPair>* tree) {
        if(tree->root)
            return tree->root->print();
    }

}
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

            bool erase(const key_type& _key) {
                return rbtree.erase(_key);
            }   

            mapped_type& operator[](const key_type& _key) {
                std::shared_ptr<TPair<key_type, mapped_type>> value = rbtree.search(_key);
                if(!value) {
                    rbtree.insert(TPair<key_type, mapped_type>(_key));
                }
                return rbtree[_key];
            }

            std::shared_ptr<TPair<key_type, mapped_type>> find(const key_type& _key) {
                return rbtree.search(_key);
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

const std::string SAVE = "Save";
const std::string LOAD = "Load";

char to_lower_case(char c) {
    if('A' <= c and c <= 'Z')
        return c - 'A' + 'a';
    return c;
}

void string_to_lower_case(std::string& s) {
    for(int i = 0; i < s.size(); ++i) {
        s[i] = to_lower_case(s[i]);
    }
}

signed main() {

    char c;
    std::string key;
    uint64_t value;

    NMap::TMap<std::string, uint64_t> mp;

    while(std::cin >> c) {
        if(c == '+') {
            std::cin >> key >> value;
            string_to_lower_case(key);

            if(mp.insert({key, value})) {
                std::cout << "OK" << std::endl;
            } else {
                std::cout << "Exist" << std::endl;
            }
        } else if(c == '-') {
            std::cin >> key;
            // std::transform(key.begin(), key.end(), key.begin(), to_lower_case);
            string_to_lower_case(key);
            
            if(mp.erase(key)) {
                std::cout << "OK" << std::endl;
            } else {
                std::cout << "NoSuchWord" << std::endl;
            }
        } else if(c == '!') {
            std::string cmd, path;
            std::cin >> cmd >> path;

            if(cmd == SAVE) {
                try {
                    std::ofstream os(path, std::ios::binary);
                    mp.save_to_file(os);
                    std::cout << "OK" << std::endl;
                } catch(...) {
                    std::cout << "ERROR:" << std::endl;
                }

            } else {
                try {
                    std::ifstream is(path, std::ios::binary);
                    mp.load_from_file(is);
                    std::cout << "OK" << std::endl;
                } catch(...) {
                    std::cout << "ERROR:" << std::endl;
                }
            }
        } else {
            std::string tmpKey;
            getline(std::cin, tmpKey);
            key = c + tmpKey;
            // std::transform(key.begin(), key.end(), key.begin(), to_lower_case);
            string_to_lower_case(key);

            std::shared_ptr<NMap::TPair<std::string, uint64_t>> ptr = mp.find(key);
            if(ptr == nullptr) {
                std::cout << "NoSuchWord" << std::endl;
            } else {
                std::cout << "OK: " << ptr->second << std::endl;
            }
        }
    }
}