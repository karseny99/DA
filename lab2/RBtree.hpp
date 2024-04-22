#include <memory>
#include <iostream>

enum colors {
    RED, 
    BLACK
};

template <class pair>
class RBTree {
    private:
        class Node {
            private:
                const int SPACES_COUNT = 2;
                const std::string red = "\033[0;31m";
                const std::string green = "\033[1;32m";
                const std::string yellow = "\033[1;33m";
                const std::string cyan = "\033[0;36m";
                const std::string magenta = "\033[0;35m";
                const std::string reset = "\033[0m";

            public:
                // pair item;
                long long value;
                colors color = BLACK;
                std::shared_ptr<Node> child[2];
                // std::shared_ptr<Node> child[0];
                // std::shared_ptr<Node> child[1];

                Node(colors _color, \
                    long long _value, \
                    std::shared_ptr<Node> _left, \
                    std::shared_ptr<Node> _right) : 
                        color(_color), 
                        value(_value) {
                            child[0] = _left;
                            child[1] = _right;
                        }
                
                ~Node() = default;

                bool value_exists(long long _value) {
                    return (value == _value) or 
                        (child[0] != nullptr and child[0]->value == _value) or 
                        (child[1] != nullptr and child[1]->value == _value);
                }   
                
                void print(int space = 0) {
                    space += 2;

                    if(child[1])
                        child[1]->print(space);
                    for(int i = SPACES_COUNT; i < space; ++i) {
                        std::cout << ' ' ;
                    }
                    if(color == RED)
                        std::cout << red << value << reset << std::endl;
                    else 
                        std::cout << value << std::endl;
                        
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


        };

        std::shared_ptr<Node> root = nullptr;

        bool is_red(std::shared_ptr<Node> node) const {
            return (node != nullptr and node->color == RED);
        }

        bool is_black(std::shared_ptr<Node> node) const {
            return (node == nullptr or node->color == BLACK);
        }
        

        std::shared_ptr<Node> rotate(std::shared_ptr<Node> currentRoot, bool direction) { // left / right
            std::shared_ptr<Node> newRoot = currentRoot->child[!direction];

            currentRoot->child[!direction] = newRoot->child[direction];
            newRoot->child[direction] = currentRoot;

            newRoot->color = currentRoot->color;
            currentRoot->color = RED;

            return newRoot;
        }    

        std::shared_ptr<Node> doubleRotate(std::shared_ptr<Node> currentRoot, bool direction) {
            currentRoot->child[!direction] = rotate(currentRoot->child[!direction], !direction);
            return rotate(currentRoot, direction);
        }

        // sets opposite color to root and its children
        void change_colors(std::shared_ptr<Node> node) { 
            node->color = static_cast<colors>(!node->color);
            if(node->child[0])
                node->child[0]->color = static_cast<colors>(!node->child[0]->color);
            if(node->child[1])
                node->child[1]->color = static_cast<colors>(!node->child[1]->color);
        }

        std::shared_ptr<Node> check_add_correctness(std::shared_ptr<Node> localRoot, bool direction) {
            
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

        std::shared_ptr<Node> _insert(std::shared_ptr<Node> currentNode, long long value, bool& is_found) {

            if(currentNode == nullptr) 
                return std::make_shared<Node>(RED, value, nullptr, nullptr);

            if(currentNode->value == value) {
                is_found = true;
                return currentNode;
            }

            bool direction = (value > currentNode->value);
            currentNode->child[direction] = _insert(currentNode->child[direction], value, is_found);

            // if(is_found)
            //     return currentNode;
            return check_add_correctness(currentNode, direction);
        }

        std::shared_ptr<Node> get_minimum(std::shared_ptr<Node> currentRoot) {
            auto mover = currentRoot;
            while(mover->child[0] != nullptr) {
                mover = mover->child[0];
            }

            return mover;
        }

        std::shared_ptr<Node> check_erase_correctness(std::shared_ptr<Node> currentNode, bool direction, bool& needBalance) {
            std::shared_ptr<Node> parent = currentNode;
            std::shared_ptr<Node> brother = currentNode->child[!direction];

            if(is_red(brother)) {
                currentNode = rotate(currentNode, direction);
                brother = parent->child[!direction];
            }

            if(brother) {

                if(is_black(brother->child[0]) and is_black(brother->child[1])) {

                    if(is_red(parent)) needBalance = false;

                    parent->color = BLACK;
                    brother->color = RED;

                } else {
                    colors parentColor = parent->color;
                    bool isRedBrotherReduction = (currentNode != parent);

                    if(is_red(brother->child[!direction])) {
                        parent = rotate(parent, direction);
                    } else {
                        parent = doubleRotate(parent, direction);
                    }

                    parent->color = parentColor;
                    parent->child[0]->color = BLACK;
                    parent->child[1]->color = BLACK;

                    if(isRedBrotherReduction) {
                        currentNode->child[direction] = parent;
                    } else {
                        currentNode = parent;
                    }

                    needBalance = false;
                }
            }
            return currentNode;
        }

        std::shared_ptr<Node> _erase(std::shared_ptr<Node> currentNode, long long value, bool& needBalance, bool& erased) {

            if(currentNode == nullptr)  {
                needBalance = false;
                erased = false;
                return nullptr;
            }

            if(currentNode->value == value) {

                // has zero or one child
                if(currentNode->child[0] == nullptr or currentNode->child[1] == nullptr) {

                    std::shared_ptr<Node> tmp;

                    if(currentNode->child[0]) tmp = currentNode->child[0];
                    if(currentNode->child[1]) tmp = currentNode->child[1];

                    // If node is red, the black height is not changing -> don't need to do anything                
                    if(is_red(currentNode)) {
                        currentNode = nullptr;
                        needBalance = false;
                    
                    // if node is black and its child is red -> simply replace colors;
                    } else if(is_red(tmp)) {
                        currentNode = nullptr;
                        tmp->color = BLACK;
                        needBalance = false;
                    }

                    return tmp;

                // has two children, find minimum in right subtree and copy its value in currentNode, 
                // then delete this minimum from right subtree
                } else {
                    std::shared_ptr<Node> tmp = get_minimum(currentNode->child[1]);
                    std::cout << tmp->value << std::endl;

                    currentNode->value = tmp->value;
                    value = tmp->value;
                }
            }

            bool direction = (value >= currentNode->value);
            
            currentNode->child[direction] = _erase(currentNode->child[direction], value, needBalance, erased);

            return needBalance ? check_erase_correctness(currentNode, direction, needBalance) : currentNode;
        } 

    public: 

        bool insert(long long value) {
            bool is_found = false;
            root = _insert(root, value, is_found);
            root->color = BLACK;
            if(is_found)
                return false;
            return true;
        }

        bool erase(long long value) {
            bool erased = true;
            bool needBalanced = true;
            root = _erase(root, value, needBalanced, erased);
            if(root)
                root->color = BLACK;
            return erased;
        }

        friend void print(RBTree<key_type, value_type>* tree);
};

template<typename key_type, typename value_type>
void print(RBTree<key_type, value_type>* tree) {
    if(tree->root)
        return tree->root->print();
}

