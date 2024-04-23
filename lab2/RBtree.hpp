#pragma once
#include <memory>
#include <iostream>
#include <fstream>
#include "map.hpp"
#include <cstring>

namespace NMap {

    enum colors {
        RED, 
        BLACK
    };

    const std::string BNODE = "BNODE";
    const std::string ENODE = "ENODE";
    const std::string red = "\033[0;31m";
    const std::string green = "\033[1;32m";
    const std::string yellow = "\033[1;33m";
    const std::string cyan = "\033[0;36m";
    const std::string magenta = "\033[0;35m";
    const std::string reset = "\033[0m";

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
                                child[0] = _left;
                                child[1] = _right;
                            }
                    
                    ~TNode() = default;

                    bool value_exists(TPair _item) {
                        return (item.first == _item.first) or 
                            (child[0] != nullptr and child[0]->item.first == _item.first) or 
                            (child[1] != nullptr and child[1]->item.first == _item.first);
                    }   
                    
                    void print(int space = 0) {
                        space += 2;

                        if(child[1])
                            child[1]->print(space);
                        for(int i = SPACES_COUNT; i < space; ++i) {
                            std::cout << ' ' ;
                        }
                        if(color == RED)
                            std::cout << red << item.first << reset << std::endl;
                        else 
                            std::cout << item.first << std::endl;
                            
                        if(child[0])
                            child[0]->print(space);
                    }

                    bool has_no_children() {
                        return (child[0] == nullptr) and (child[1] == nullptr);
                    }

                    bool has_one_child() {
                        return  (child[0] != nullptr) ^ (child[1] != nullptr);
                    }

                    bool has_two_children() {
                        return (child[0] != nullptr) and (child[1] != nullptr);
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
                        if(child[0]) child[0]->write_data_to_file(os);
                        else os.write(ENODE.c_str(), ENODE.size()); // 5 bytes
                        if(child[1]) child[1]->write_data_to_file(os);
                        else os.write(ENODE.c_str(), ENODE.size()); // 5 bytes
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
                if(TNode->child[0])
                    TNode->child[0]->color = static_cast<colors>(!TNode->child[0]->color);
                if(TNode->child[1])
                    TNode->child[1]->color = static_cast<colors>(!TNode->child[1]->color);
            }

            std::shared_ptr<TNode> check_add_correctness(std::shared_ptr<TNode> localRoot, bool direction) {
                
                if(is_red(localRoot->child[direction])) {
                    
                    // parent is red, uncle is red -> simply switch colors
                    if(is_red(localRoot->child[!direction])) {
                        if(is_red(localRoot->child[direction]->child[direction]) or is_red(localRoot->child[direction]->child[!direction])) {
                            std::cout << "->Uncle is red, changing colors" << std::endl;
                            change_colors(localRoot); 
                        }

                    // parent is red, uncle is black -> need rotate
                    } else {
                        // order is right
                        if(is_red(localRoot->child[direction]->child[direction])) {
                            std::cout << "->Uncle is black, rotating" << std::endl;

                            localRoot = rotate(localRoot, !direction);
                        // order is wrong, need to rotate twice
                        } else if(is_red(localRoot->child[direction]->child[!direction])) {
                            std::cout << "->Uncle is black, rotating" << std::endl;

                            localRoot = doubleRotate(localRoot, !direction);
                        }
                    }
                    
                } 

                return localRoot;
            } 

            std::shared_ptr<TNode> _insert(std::shared_ptr<TNode> currentTNode, TPair item, bool& is_found) {

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
                while(mover->child[0] != nullptr) {
                    mover = mover->child[0];
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

                    if(is_black(brother->child[0]) and is_black(brother->child[1])) {

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
                        parent->child[0]->color = BLACK;
                        parent->child[1]->color = BLACK;

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
                    if(currentTNode->child[0] == nullptr or currentTNode->child[1] == nullptr) {

                        std::shared_ptr<TNode> tmp;

                        if(currentTNode->child[0]) tmp = currentTNode->child[0];
                        if(currentTNode->child[1]) tmp = currentTNode->child[1];

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
                        std::shared_ptr<TNode> tmp = get_minimum(currentTNode->child[1]);
                        // std::cout << tmp->value << std::endl;

                        currentTNode->item = tmp->item;
                        key = tmp->item.first;
                    }
                }

                bool direction = (key >= currentTNode->item.first);
                
                currentTNode->child[direction] = _erase(currentTNode->child[direction], key, needBalance, erased);

                return needBalance ? check_erase_correctness(currentTNode, direction, needBalance) : currentTNode;
            } 

            TPair::second_type& _search(TPair::first_type key, std::shared_ptr<TNode> currentTNode) {
                // if tree doesn't have this key => adding
                if(currentTNode == nullptr) {
                    insert((TPair(key)));
                    return search(key);
                }
                if(key == currentTNode->item.first) {
                    return currentTNode->item.second;
                }
                bool direction = (key >= currentTNode->item.first);
                return _search(key, currentTNode->child[direction]);
            }

        public: 

            TRBTree() = default;

            bool insert(TPair item) {
                bool is_found = false;
                root = _insert(root, item, is_found);
                root->color = BLACK;
                if(is_found)
                    return false;
                return true;
            }

            bool erase(TPair::first_type key) {
                bool erased = true;
                bool needBalanced = true;
                root = _erase(root, key, needBalanced, erased);
                if(root)
                    root->color = BLACK;
                return erased;
            }

            TPair::second_type& search(TPair::first_type key) {
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
                    std::cout << buffer << std::endl;

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
                    
                    std::cout << size << ' ' << key << ' ' << value << ' ' << nodeColor << std::endl;
                    TPair tmpData;
                    tmpData.first = key;
                    tmpData.second = value;
                    std::shared_ptr<TNode> _root = std::make_shared<TNode>(nodeColor, tmpData, nullptr, nullptr);
                    
                    _root->child[0] = load_from_file(_root, is);
                    _root->child[1] = load_from_file(_root, is);

                    return _root;
                } else {
                    char buffer[ENODE.size()];
                    is.read(buffer, (ENODE.size()));
                    return nullptr;
                }

            }

            void load_from_file(std::ifstream& is) {
                root = load_from_file(root,is);
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
