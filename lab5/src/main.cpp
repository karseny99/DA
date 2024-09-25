#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <array>
#include <unordered_map>

namespace NSuffixTree {


class SuffixTree {

private:

    std::size_t INF = 1e9;

    struct SuffixNode {
        std::size_t l;
        std::size_t r;

        std::shared_ptr<SuffixNode> suffixLink;
        std::shared_ptr<SuffixNode> parent;

        // std::vector<std::shared_ptr<SuffixNode>> nodes;
        // std::array<int, ALPHABET_SIZE> nodes;

        std::unordered_map<int, std::shared_ptr<SuffixNode>> nodes;

        SuffixNode() : l(0), r(0), suffixLink(nullptr), parent(nullptr) {}
        SuffixNode(
                    std::size_t _l, 
                    std::size_t _r,
                    std::shared_ptr<SuffixNode> _suffixLink,
                    std::shared_ptr<SuffixNode> _parent
                ) : l(_l), r(_r), suffixLink(_suffixLink), parent(_parent) {} 

        void print() {
            std::cout << l << ' ' << r << ' ' << (suffixLink != nullptr) << ' ';
            for(auto& child : nodes) {
                std::cout << static_cast<char>(child.first);
            }
            std::cout << std::endl;

            for(auto& child : nodes) {
                child.second->print();
            }
        } 
    };

public:

    std::shared_ptr<SuffixNode> root;
    std::shared_ptr<std::size_t> end;
    std::string text;

    // Current point
    std::shared_ptr<SuffixNode> currentNode;
    std::size_t currentEdge;
    std::size_t currentShift;
    std::shared_ptr<SuffixNode> needSuffixLink;

    std::size_t reminder = 0;

private:

    std::size_t edge_length(std::shared_ptr<SuffixNode> node) const {
        return node->r - node->l;
    }

    void add_suffix_link(std::shared_ptr<SuffixNode> node) {
        if(needSuffixLink == root) 
            return;

        needSuffixLink->suffixLink = node;
        needSuffixLink = node;
    }

    bool walk_down(std::shared_ptr<SuffixNode> node) {
        if(currentShift >= edge_length(node)) {
            currentEdge += edge_length(node);
            currentShift -= edge_length(node);
            currentNode = node;
            return true; 
        }
        return false;
    }

public:

    SuffixTree() {
        currentNode = root = std::make_shared<SuffixNode>(-1, -1, nullptr, nullptr);
        currentEdge = currentShift = 0;
        needSuffixLink = nullptr;
    }

    void addSymbol(char symbol) {

        text.push_back(symbol);
        ++reminder;
        needSuffixLink = currentNode;

        while(reminder > 0) {
            if(currentShift == 0) 
                currentEdge = text.size() - 1;

            if(currentNode->nodes[text[currentEdge]] == nullptr) {
                // std::cout << "IM HERE " << text.size() << ' ' << text.back() << std::endl;
                std::shared_ptr<SuffixNode> new_node = std::make_shared<SuffixNode>(
                    text.size() - 1,
                    INF,
                    nullptr,
                    currentNode
                );

                currentNode->nodes[text[currentEdge]] = new_node;
                
                /* Rule 2
                If we split an edge and insert a new node, 
                and if that is not the first node created during the current step, 
                we connect the previously inserted node and the new node through a special pointer, 
                a suffix link
                */
                add_suffix_link(currentNode);

            } else {    
                std::shared_ptr<SuffixNode> node = currentNode->nodes[text[currentEdge]];

                /* Observation 2 ?????????????????????????????????
                If at some point active_length is greater or equal to the length of current edge (edge_length), 
                we move our active point down until edge_length is strictly greater than active_length
                */
                if(walk_down(node)) continue;

                /* Observation 1
                When the final suffix we need to insert is found to exist in the tree already, 
                the tree itself is not changed at all 
                (we only update the active point and remainder)
                */
                if(text[node->l + currentShift] == symbol) {
                    ++currentShift;
                    // std::cout << "IM HERE " << node->l << ' ' << currentShift << ' ' << text.back() << std::endl;

                    /*  Observation 3
                    if there is an internal node marked as needing suffix link, 
                    we must connect that node with our current active node through a suffix link.
                    */
                    add_suffix_link(currentNode);
                    break;
                }

                // New internal node
                std::shared_ptr<SuffixNode> internalNode = std::make_shared<SuffixNode>(
                    node->l, 
                    node->l + currentShift,
                    nullptr,
                    currentNode
                );
                currentNode->nodes[text[currentEdge]] = internalNode;
                
                // New leaf
                internalNode->nodes[symbol] = std::make_shared<SuffixNode>(
                    text.size() - 1, 
                    INF,
                    nullptr,
                    internalNode
                );

                node->parent = internalNode;
                node->l += currentShift;
                internalNode->nodes[text[node->l]] = node;
                add_suffix_link(internalNode);
            }
            --reminder;

            
            /* Rule 1
            If after an insertion from the active node = root, the active length is greater than 0, then:
            - active node is not changed
            - active length is decremented
            - active edge is shifted right (to the first character of the next suffix we must insert)
            */
            if(currentNode == root and currentShift > 0) {
                --currentShift;
                currentEdge = text.size() - reminder;

            /* Rule 3
            After an insert from the active node which is not the root node, 
            we must follow the suffix link and set the active node to the node it points to. 
            If there is no a suffix link, 
            set the active node to the root node. 
            Either way, active edge and active length stay unchanged
            */
            } else {
                currentNode = (currentNode->suffixLink != nullptr) ? currentNode->suffixLink : root;
            }
        }
    }

    void print() const {
        root->print();
    }
};


}

// #include <iostream>
// #include <memory>
// #include <string>
// #include <vector>
// #include <array>


int main() {
    // std::ios::sync_with_stdio(false);
    // std::cin.tie(0);

    NSuffixTree::SuffixTree tr;
    // tr.addSymbol('a');
    // tr.addSymbol('b');
    // tr.addSymbol('c');
    // tr.addSymbol('a');
    // tr.addSymbol('$');

    std::string s = "abcab$";
    for(auto e : s ) {
        tr.addSymbol(e);
    }

    // for(auto e : tr.root->nodes) {
    //     std::cout << static_cast<char>(e.first) << std::endl;
    // }

    tr.print();
    // std::string t, p;
    // std::cin >> t >> p;
    // std::cout << t << ' ' << p << '\n';
}