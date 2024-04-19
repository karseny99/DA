#include <memory>
#include <iostream>

enum colors {
    RED, 
    BLACK
};

// template <class T>
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
                colors color = BLACK;
                long long value;  

                std::shared_ptr<Node> leftChild;
                std::shared_ptr<Node> rightChild;
                std::shared_ptr<Node> parent;

                Node(colors _color, long long _value, std::shared_ptr<Node> _left, std::shared_ptr<Node> _right, std::shared_ptr<Node> _parent) :
                    color(_color), 
                    value(_value),
                    leftChild(_left),
                    rightChild(_right),
                    parent(_parent) {}
                
                ~Node() = default;

                bool value_exists(long long _value) {
                    return (value == _value) or 
                        (leftChild != nullptr and leftChild->value == _value) or 
                        (rightChild != nullptr and rightChild->value == _value);
                }   

                std::shared_ptr<Node> get_uncle() {
                    std::shared_ptr<Node> grandParent = parent->parent;
                    if(!grandParent)    
                        return nullptr;
                    return (grandParent->rightChild != parent) ? grandParent->rightChild : grandParent->leftChild;
                }

                std::shared_ptr<Node> get_root() {
                    if(!parent) 
                        return std::make_shared<Node>(*this);
                    auto mover = parent;
                    while(mover->parent != nullptr) {
                        mover = mover->parent;
                    }
                    return mover;
                }

                void print(int space = 0) {
                    space += 2;

                    if(rightChild)
                        rightChild->print(space);
                    for(int i = SPACES_COUNT; i < space; ++i) {
                        std::cout << ' ' ;
                    }
                    if(color == RED)
                        std::cout << red << value << reset << std::endl;
                    else 
                        std::cout << value << std::endl;
                        
                    if(leftChild)
                        leftChild->print(space);
                }

        };

        std::shared_ptr<Node> root = nullptr;
    public: 

        RBTree() = default;

        RBTree(long long _value) {
            root = std::make_shared<Node>(BLACK, _value, nullptr, nullptr, nullptr);
        }

        bool isEmpty() const {
            return root == nullptr;
        }

    private:

        void rotate_right(std::shared_ptr<Node> oldRoot) { 
            std::cout << "----> Right rotate " << oldRoot->value << std::endl;

            auto newRoot = oldRoot->leftChild;
            oldRoot->leftChild = newRoot->rightChild;

            if(oldRoot->leftChild) 
                oldRoot->leftChild->parent = oldRoot;
            
            newRoot->parent = oldRoot->parent;

            if(oldRoot->parent == nullptr) 
                root = newRoot;
            else if(oldRoot == oldRoot->parent->leftChild) 
                oldRoot->parent->leftChild = newRoot;
            else 
                oldRoot->parent->rightChild = newRoot;
            
            newRoot->rightChild = oldRoot;
            oldRoot->parent = newRoot;          
        }

        void rotate_left(std::shared_ptr<Node> oldRoot) { 
            std::cout << "----> Left rotate " << oldRoot->value << std::endl;

            auto newRoot = oldRoot->rightChild;
            oldRoot->rightChild = newRoot->leftChild;

            if(oldRoot->rightChild) 
                oldRoot->rightChild->parent = oldRoot;
            
            newRoot->parent = oldRoot->parent;

            if(oldRoot->parent == nullptr) 
                root = newRoot;
            else if(oldRoot == oldRoot->parent->leftChild) 
                oldRoot->parent->leftChild = newRoot;
            else 
                oldRoot->parent->rightChild = newRoot;
            
            newRoot->leftChild = oldRoot;
            oldRoot->parent = newRoot;    
        }


        std::shared_ptr<Node> search(long long _value) {
            std::shared_ptr<Node> currentNode = root;
            std::shared_ptr<Node> prevNode = nullptr;

            while(currentNode != nullptr) {
                if(currentNode->value == _value) {
                    break;
                } else if(currentNode->value < _value) {
                    prevNode = currentNode;
                    currentNode = currentNode->rightChild;
                } else {
                    prevNode = currentNode;
                    currentNode = currentNode->leftChild;
                }
            }

            // root-case
            if(prevNode == nullptr)
                return currentNode;
            return prevNode;
        }

        void check_add_correctness(std::shared_ptr<Node> currentNode) { // private
            std::cout << "-----------------" << std::endl;
            print(this);
            std::cout << "-----------------" << std::endl;


            if(currentNode->parent->color == BLACK) {
                std::cout << "-->Don't need to balance tree, parent is black\n";

            } else if(currentNode->get_uncle() == nullptr or currentNode->get_uncle()->color == BLACK) {

                if(currentNode->get_uncle() != nullptr)
                    std::cout << "-->Need to balance, uncle is black(" << currentNode->get_uncle()->value << ")\n";
                else 
                    std::cout << "-->Need to balance, uncle is black\n";
                
                auto grandParent = currentNode->parent->parent;
                if(grandParent->rightChild == currentNode->get_uncle()) {
                    if(currentNode->parent->rightChild == currentNode) {
                        std::cout << "--->I'm a right node, need to swap\n";
                        rotate_left(currentNode->parent);
                        currentNode = currentNode->leftChild;
                    }
                    std::swap(currentNode->parent->color, currentNode->parent->parent->color);
                    rotate_right(grandParent);
                    root = currentNode->get_root();
                    std::cout << "-----------------" << std::endl;
                    print(this);
                    std::cout << "-----------------" << std::endl;
                } else {
                    if(currentNode->parent->leftChild == currentNode) {
                        std::cout << "--->I'm a left node, need to swap\n";
                        rotate_right(currentNode->parent);
                        currentNode = currentNode->rightChild;
                    }
                    std::swap(currentNode->parent->color, currentNode->parent->parent->color);
                    rotate_left(grandParent);
                    std::cout << "================= " << std::endl;
                    print(this);
                    std::cout << "================= " << std::endl;
                }

            } else if(currentNode->get_uncle() != nullptr and currentNode->get_uncle()->color == RED) {
                std::cout << "-->Need to balance, parent is red, uncle is red\n";
                currentNode->parent->color = BLACK;
                std::swap(currentNode->get_uncle()->color, currentNode->parent->parent->color);
                if(currentNode->parent->parent->parent != nullptr and currentNode->parent->parent->parent->color == RED) {
                    check_add_correctness(currentNode->parent->parent);
                }

            }

            root = currentNode->get_root();
            std::cout << "Root is " << root->value << std::endl;
            root->color = BLACK;
        }

        void check_remove_correctness(std::shared_ptr<Node> currentNode) {
            while(currentNode != root and currentNode->color == BLACK) {
                
                // I'm a left child
                if(currentNode->parent->leftChild == currentNode) {
                    auto brother = currentNode->parent->rightChild;

                    // brother is red
                    if(brother->color == RED) {
                        brother->color = BLACK;
                        currentNode->parent->color = RED;
                        rotate_left(currentNode->parent);
                    
                    // brother is black, his children are black
                    } else if(brother->color == BLACK and 
                        ((brother->leftChild == nullptr and brother->rightChild == nullptr) 
                            or (brother->leftChild != nullptr and brother->leftChild->color == BLACK
                                    and brother->rightChild != nullptr and brother->rightChild->color == BLACK))) 
                    {
                        brother->color = RED;
                        currentNode = currentNode->parent;
                                        
                    // brother is black, brother->left (red) and brother->right (black) 
                    } else {
                        if(brother->color == BLACK and brother->leftChild->color == RED and (brother->rightChild == nullptr or brother->rightChild->color == BLACK)) {
                            brother->leftChild->color = BLACK;
                            brother->color = RED;
                            rotate_right(brother);
                            brother = currentNode->parent->rightChild;
                        }
                        brother->color = currentNode->parent->color;
                        currentNode->parent->color = BLACK;
                        brother->rightChild->color = BLACK;

                        rotate_left(currentNode->parent);
                        currentNode = root;
                    }
                    // brother is black, brother->right (red)
                    // } else if(brother->color == BLACK and (brother->rightChild != nullptr and brother->rightChild->color == RED)) {
                    // }
                } else {
                    auto brother = currentNode->parent->leftChild;
                    // brother is red
                    if(brother->color == RED) {
                        brother->color = BLACK;
                        currentNode->parent->color = RED;
                        rotate_right(currentNode->parent);
                    
                    // brother is black, his children are black
                    } else if(brother->color == BLACK and 
                        ((brother->leftChild == nullptr and brother->rightChild == nullptr) 
                            or (brother->leftChild != nullptr and brother->leftChild->color == BLACK
                                    and brother->rightChild != nullptr and brother->rightChild->color == BLACK))) 
                    {
                        brother->color = RED;
                        currentNode = currentNode->parent;
                                        
                    // brother is black, brother->left (red) and brother->right (black) 
                    } else { 
                        if(brother->color == BLACK and brother->leftChild->color == RED and (brother->rightChild == nullptr or brother->rightChild->color == BLACK)) {
                            brother->leftChild->color = BLACK;
                            brother->color = RED;
                            rotate_left(brother);
                            brother = currentNode->parent->rightChild;
                        }
                        brother->color = currentNode->parent->color;
                        currentNode->parent->color = BLACK;
                        brother->rightChild->color = BLACK;

                        rotate_right(currentNode->parent);
                        currentNode = root;
                    // brother is black, brother->right (red)
                    // } else if(brother->color == BLACK and (brother->rightChild != nullptr and brother->rightChild->color == RED)) {
                    // }
                    }

                    std::cout << "================" << std::endl;
                    print(this);
                }
            }
            currentNode->color = BLACK;
        }

        void transplant(std::shared_ptr<Node> currentNode, std::shared_ptr<Node> child) {
            // root-case
            if(currentNode->parent == nullptr) {
                root = child;
            } else if(currentNode == currentNode->parent->leftChild) {
                currentNode->parent->leftChild = child;
            } else {
                currentNode->parent->rightChild = child;
            }

            if(child)
                child->parent = currentNode->parent;
        }

        std::shared_ptr<Node> get_minimum(std::shared_ptr<Node> currentRoot) {
            if(currentRoot->leftChild == nullptr) {
                return currentRoot;
            } 

            auto mover = currentRoot->leftChild;
            while(mover->leftChild != nullptr) {
                mover = mover->leftChild;
            }

            return mover;
        }

    public: 

        std::shared_ptr<Node> find(long long _value) {
            std::shared_ptr<Node> foundNode = search(_value);
            if(!foundNode) 
                return nullptr;
            else if(foundNode->value == _value) 
                return foundNode;
            else if(foundNode->rightChild != nullptr and foundNode->rightChild->value == _value)
                return foundNode->rightChild;
            else if(foundNode->leftChild != nullptr and foundNode->leftChild->value == _value)
                return foundNode->leftChild;
            return nullptr;
        }


        bool remove(long long _value) {
            if(isEmpty()) 
                return false;

            std::shared_ptr<Node> foundParent = search(_value);
            bool valueExists = foundParent->value_exists(_value);
            if(!valueExists) {
                return false;
            }
            
            std::shared_ptr<Node> removeNode = foundParent;
            if(foundParent->value < _value) {
                removeNode = foundParent->rightChild;
            } else if(foundParent->value > _value) {
                removeNode = foundParent->leftChild;
            }
            std::cout << "-> Removing " << removeNode->value << std::endl;
            colors removeColor = removeNode->color;
           
            std::shared_ptr<Node> successorNode;

            if(removeNode->leftChild == nullptr) {
                std::cout << "-->leftChild is null or children null " << std::endl;
                successorNode = removeNode->rightChild;
                transplant(removeNode, successorNode);
            } else if(removeNode->rightChild == nullptr) {
                std::cout << "-->Right child is null" << std::endl;
                successorNode = removeNode->leftChild;
                transplant(removeNode, successorNode);
            } else {
                std::cout << "-->Node has two children" << std::endl;

                std::shared_ptr<Node> minimumNode = get_minimum(removeNode->rightChild);
                removeColor = minimumNode->color;
                successorNode = minimumNode->rightChild;

                if(minimumNode->parent != removeNode) {
                    transplant(minimumNode, minimumNode->rightChild);
                    minimumNode->rightChild = removeNode->rightChild;
                    minimumNode->rightChild->parent = minimumNode;
                    transplant(removeNode, minimumNode);
                }

                transplant(removeNode, minimumNode);
                minimumNode->leftChild = removeNode->leftChild;
                minimumNode->leftChild->parent = minimumNode;
                minimumNode->color = removeNode->color;

            }
            std::cout << "===========================" << std::endl;
            print(this);
            if(removeColor == BLACK) {
                check_remove_correctness(successorNode);
            } 
            
            return true;


        }

        bool add(long long _value) {

            if(isEmpty()) {
                root = std::make_shared<Node>(BLACK, _value, nullptr, nullptr, nullptr);
                return true;
            }

            std::shared_ptr<Node> foundParent = search(_value);
            bool valueExists = foundParent->value_exists(_value);
            if(valueExists) {
                return false;
            }
            std::shared_ptr<Node> insertedNode = (_value > foundParent->value) ? foundParent->rightChild : foundParent->leftChild;
            std::cout << "-> " << _value << std::endl;
            insertedNode = std::make_shared<Node>(RED, _value, nullptr, nullptr, foundParent);
            if(_value > foundParent->value) {
                foundParent->rightChild = insertedNode;
            } else {
                foundParent->leftChild = insertedNode;
            }

            check_add_correctness(insertedNode);

            std::cout << "-----> " <<  insertedNode->value << ' ' << insertedNode->parent->value << ' ' << std::endl;
            return true;
        }   

        friend void print(RBTree* tree);


};


void print(RBTree* tree) {
    return tree->root->print();
}

