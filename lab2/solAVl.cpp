#include <memory>
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdint>

namespace NMap {

    const int LEFT = 0;
    const int RIGHT = 1; 

    const std::string BNODE = "BNODE";
    const std::string ENODE = "ENODE";
    const std::string red = "\033[0;31m";
    const std::string green = "\033[1;32m";
    const std::string yellow = "\033[1;33m";
    const std::string cyan = "\033[0;36m";
    const std::string magenta = "\033[0;35m";
    const std::string reset = "\033[0m";

    template <class TPair>
    class TAVLTree {
        private:
            class TNode {
                private:
                    const int SPACES_COUNT = 2;
                public:
                    TPair item;
                    std::shared_ptr<TNode> child[2];
                    int height;
                    TNode(int _height, \
                        TPair _item, \
                        std::shared_ptr<TNode> _left, \
                        std::shared_ptr<TNode> _right) : 
                            height(_height), 
                            item(_item) {
                                child[LEFT] = _left;
                                child[RIGHT] = _right;
                            }

                    int get_diff() {
                        int l = 0, r = 0;
                        if(child[LEFT]) 
                            l = child[LEFT]->height;
                        if(child[RIGHT]) 
                            r = child[RIGHT]->height;
                        return l - r;
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

                        // node's height
                        int hght = height;
                        os.write(reinterpret_cast<const char*>((&hght)), sizeof(hght));  

                        if(child[LEFT]) child[LEFT]->write_data_to_file(os);
                        else os.write(ENODE.c_str(), ENODE.size()); // 5 bytes
                        if(child[RIGHT]) child[RIGHT]->write_data_to_file(os);
                        else os.write(ENODE.c_str(), ENODE.size()); // 5 bytes
                    }

                                
                    void print(int space = 0) {
                        space += 2;

                        if(child[RIGHT])
                            child[RIGHT]->print(space);
                        for(int i = SPACES_COUNT; i < space; ++i) {
                            std::cout << ' ' ;
                        }

                        // std::cout << "--> " << get_diff() << std::endl;
                        if(get_diff() == 0)
                            std::cout << item.first << reset << std::endl;
                        else if(get_diff() == 1) 
                            std::cout << green << item.first << reset << std::endl;
                        else if(get_diff() == -1)
                            std::cout << cyan << item.first << reset << std::endl;
                        else if(get_diff() == 2) 
                            std::cout << red << item.first << reset << std::endl;
                        else if(get_diff() == -2)
                            std::cout << yellow << item.first << reset << std::endl;
                            
                        if(child[LEFT])
                            child[LEFT]->print(space);
                    }
            };

            int height(std::shared_ptr<TNode> currentRoot) {
                return (currentRoot ? currentRoot->height : 0);
            }

            int get_diff(std::shared_ptr<TNode> currentRoot) {
                return (currentRoot ? currentRoot->get_diff() : 0);
            }

            std::shared_ptr<TNode> rotate(std::shared_ptr<TNode> currentRoot, bool direction) { // left / right
                std::shared_ptr<TNode> newRoot = currentRoot->child[!direction];

                currentRoot->child[!direction] = newRoot->child[direction];
                newRoot->child[direction] = currentRoot;

                currentRoot->height = std::max(height(currentRoot->child[direction]), height(currentRoot->child[!direction])) + 1;
                newRoot->height = std::max(height(currentRoot->child[!direction]), currentRoot->height) + 1 ;

                return newRoot;
            }  

            std::shared_ptr<TNode> double_rotate(std::shared_ptr<TNode> currentRoot, bool direction) {
                currentRoot->child[!direction] = rotate(currentRoot->child[!direction], !direction);
                return rotate(currentRoot, direction);
            }

            std::shared_ptr<TNode> check_add_correctness(std::shared_ptr<TNode> localRoot, bool direction) {
                
                // zero balance or +-1 balance => passing
                if(localRoot->get_diff() == 0 or localRoot->get_diff() == 1 or localRoot->get_diff() == -1) {
                    return localRoot;
                } 
                // +-2 balance => rotating
                
                // right order
                if(height(localRoot->child[direction]->child[direction]) > height(localRoot->child[direction]->child[!direction])) {
                    localRoot = rotate(localRoot, !direction);
                } else {
                    localRoot = double_rotate(localRoot, !direction);
                }
                
                return localRoot;
            } 

            void update_subtree_height(std::shared_ptr<TNode> node) {
                int l = (node->child[LEFT] ? node->child[LEFT]->height : 0);
                int r = (node->child[RIGHT] ? node->child[RIGHT]->height : 0);
                node->height = std::max(l, r) + 1;
            }

            std::shared_ptr<TNode> _insert(std::shared_ptr<TNode> currentNode, const TPair& item, bool& is_found) {

                if(currentNode == nullptr) 
                    return std::make_shared<TNode>(1, item, nullptr, nullptr);

                if(currentNode->item.first == item.first) {
                    is_found = true;
                    return currentNode;
                }

                bool direction = (item.first > currentNode->item.first);
                currentNode->child[direction] = _insert(currentNode->child[direction], item, is_found);

                // update balance
                update_subtree_height(currentNode);

                return check_add_correctness(currentNode, direction);
            }

            std::shared_ptr<TNode> get_minimum(std::shared_ptr<TNode> currentRoot) {
                auto mover = currentRoot;
                while(mover->child[LEFT] != nullptr) {
                    mover = mover->child[LEFT];
                }

                return mover;
            }

            std::shared_ptr<TNode> check_erase_correctness(std::shared_ptr<TNode> currentNode, bool& needBalance, bool direction) {
                
                if(get_diff(currentNode) == -1 or get_diff(currentNode) == 1) {
                    needBalance = false;
                }

                if(get_diff(currentNode) == 2 or get_diff(currentNode) == -2) {
                    if(height(currentNode->child[direction]->child[direction]) > height(currentNode->child[direction]->child[!direction])) {
                        currentNode = rotate(currentNode, !direction);
                    } else {
                        currentNode = double_rotate(currentNode, !direction);
                    }
                }

                return currentNode;
            }

            std::shared_ptr<TNode> _erase(std::shared_ptr<TNode> currentNode, TPair::first_type key, bool& needBalance, bool& erased) {

                // there's no such node
                if(currentNode == nullptr)  {
                    needBalance = false;
                    erased = false;
                    return nullptr;
                }

                if(currentNode->item.first == key) {
                    if(currentNode->child[LEFT] == nullptr or currentNode->child[RIGHT] == nullptr) {
                        std::shared_ptr<TNode> tmp;

                        if(currentNode->child[LEFT]) tmp = currentNode->child[LEFT];
                        if(currentNode->child[RIGHT]) tmp = currentNode->child[RIGHT];

                        return tmp;
                    } else {
                        std::shared_ptr<TNode> tmp = get_minimum(currentNode->child[RIGHT]);

                        currentNode->item = tmp->item;
                        key = tmp->item.first;
                    }
                }

                bool direction = (key >= currentNode->item.first);
                
                currentNode->child[direction] = _erase(currentNode->child[direction], key, needBalance, erased);
                --currentNode->height;
                return needBalance ? check_erase_correctness(currentNode, direction, needBalance) : currentNode;
            } 

            std::shared_ptr<TPair> _search(const TPair::first_type& key, std::shared_ptr<TNode> currentNode) {
                if(currentNode == nullptr) {
                    return nullptr;
                }
                if(key == currentNode->item.first) {
                    return std::make_shared<TPair>(currentNode->item);
                }
                bool direction = (key >= currentNode->item.first);
                return _search(key, currentNode->child[direction]);
            }


        private:

            std::shared_ptr<TNode> root = nullptr;

        public:

            bool insert(const TPair& item) {
                bool is_found = false;
                root = _insert(root, item, is_found);
                if(is_found)
                    return false;
                return true;
            }
        
            bool erase(const TPair::first_type& key) {
                bool erased = true;
                bool needBalanced = true;
                root = _erase(root, key, needBalanced, erased);
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

                    // read height                    
                    int height;
                    is.read(reinterpret_cast<char *>(&height), sizeof(height));

                    
                    TPair tmpData;
                    tmpData.first = key;
                    tmpData.second = value;
                    std::shared_ptr<TNode> _root = std::make_shared<TNode>(height, tmpData, nullptr, nullptr);
                    
                    _root->child[LEFT] = load_from_file(_root, is);
                    _root->child[RIGHT] = load_from_file(_root, is);

                    return _root;
                } else if(is.peek() == 'E') {
                    char buffer[ENODE.size()];
                    is.read(buffer, (ENODE.size()));
                    return nullptr;
                }

            }

            TPair& operator=(TAVLTree& other) {
                root = other.root;
            }

            TPair::second_type& operator[](TPair::first_type _key) {
                return get_lvalue(root, _key);
            }

            void load_from_file(std::ifstream& is) {
                root = load_from_file(root, is);
            }

            void clear() {
                root = nullptr;
            }

            friend void print<TPair>(TAVLTree<TPair>* tree);
    };

    template<class TPair>
    void print(TAVLTree<TPair>* tree) {
        if(tree->root)
            return tree->root->print();
    }
   
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

            TAVLTree<TPair<key_type, mapped_type>> avltree;

        public:

            bool insert(TPair<key_type, mapped_type> item) {
                return avltree.insert(item);
            }

            bool erase(const key_type& _key) {
                return avltree.erase(_key);
            }   

            std::shared_ptr<TPair<key_type, mapped_type>> find(const key_type& _key) {
                return avltree.search(_key);
            }

            void save_to_file(std::ofstream& os) {
                avltree.save_to_file(os);
            }

            void load_from_file(std::ifstream& is) {
                avltree.clear();
                avltree.load_from_file(is);
            }
    };

}


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