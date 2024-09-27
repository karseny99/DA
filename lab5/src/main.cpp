
// #include "../include/SufTree.hpp"

// #include <iostream>
// #include <memory>
// #include <string>
// #include <vector>
// #include <array>

/*

Поправить, что нет онлайн расширения, а сразу по всей данной строке строиться дерево (sufTree.hpp)

*/

#include <iostream>
#include <string>
#include <array>
#include <map>
#include <algorithm>


namespace NSuffixTree {

const static std::size_t ALPHABET_SIZE = 26 + 1;
const static std::size_t INF = 1e9;
const static std::size_t undefinedNode = std::size_t(-1);

class SuffixTree {

private:
    struct SuffixNode {
        std::size_t left;
        std::size_t length;

        std::size_t suffixLink = 0;

        std::map<char, std::size_t> childs;

        SuffixNode() : left(0), length(0) {  }
        SuffixNode(
                    std::size_t _l, 
                    std::size_t _len
                ) : left(_l), length(_len) {  } 
    };

private:

    std::vector<SuffixNode> nodes;
    std::string text;
    std::size_t size = 0;
    std::size_t nodes_len = 0;

    std::size_t currentNode = 0;
    std::size_t reminder = 0;

    // std::size_t idx_from_char(char edge) const {
    //     return (edge != '$') ? (edge - 'a') : (ALPHABET_SIZE - 1);;
    // }

    std::size_t move(std::size_t node, char c) {
        if(nodes[node].childs.count(c) == 0) {
            return undefinedNode;
        }
        return nodes[node].childs[c];
    }

    void move_node() {
        while(1) {
            char c = text[size - reminder];
            std::size_t nextNode = move(currentNode, c);

            if(nextNode == undefinedNode) return;

            if(nodes[nextNode].length < reminder) {
                reminder -= nodes[nextNode].length;
                currentNode = nextNode;
            } else return;
        }
    }

    std::size_t create_suffix_node(std::size_t left = 0, std::size_t length = INF) {
        nodes.push_back(SuffixNode(left, length));
        return ++nodes_len;
    }

public:

    SuffixTree(const std::string& s) {
        size = 0;
        text = s;
        create_suffix_node(0, INF);
        for(char c : text) {
            add_symbol(c);
        }
    }

    void add_symbol(char symbol) {

        ++reminder;
        ++size;
        std::size_t previousNode = 0;

        while(reminder > 0) {
            move_node();
            char c = text[size - reminder];

            std::size_t nextNode = move(currentNode, c);
            if(nextNode == undefinedNode) {

                nodes[currentNode].childs[c] = create_suffix_node(size - reminder);
                nodes[previousNode].suffixLink = currentNode;

                previousNode = currentNode;
                
            } else {

                /* Rule 2
                    If we split an edge and insert a new node, 
                    and if that is not the first node created during the current step, 
                    we connect the previously inserted node and the new node through a special pointer, 
                    a suffix link
                */

               /* ??? size - 1 ======= nodes[currentNode].left + reminder - 1*/
                if(text[nodes[nextNode].left + reminder - 1] != symbol) {
                    std::size_t internalNode = create_suffix_node(nodes[nextNode].left, reminder - 1);
                    std::size_t leaf = create_suffix_node(size - 1, INF);

                    nodes[nextNode].left += reminder - 1;
                    nodes[nextNode].length -= reminder - 1;
                    nodes[currentNode].childs[c] = internalNode;

                    nodes[internalNode].childs[text[nodes[nextNode].left + reminder - 1]] = nextNode;
                    nodes[internalNode].childs[c] = leaf;
                    
                    nodes[previousNode].suffixLink = internalNode;
                    previousNode = internalNode;
                } else {


                    nodes[previousNode].suffixLink = currentNode;
                    return;

                }

            }

            /* Rule 3
                After an insert from the active node which is not the root node, 
                we must follow the suffix link and set the active node to the node it points to. 
                If there is no a suffix link, 
                set the active node to the root node. 
                Either way, active edge and active length stay unchanged
            */
            if(currentNode == 0) {
                --reminder;
            } else {
                currentNode = nodes[currentNode].suffixLink;
            }

        }
    }

    void print() const {
        for(int i = 0; i < nodes.size(); ++i) {
            std::cout << nodes[i].left << ' ' << nodes[i].length << ' ' << text[nodes[i].left] << ' ' << (nodes[i].suffixLink != 0) << std::endl;
        }
    }
};

// /*

// Поправить, что нет онлайн расширения, а сразу по всей данной строке строиться дерево

// */

}


int main() {
    // std::ios::sync_with_stdio(false);
    // std::cin.tie(0);

    std::string s = "abcabxabc$";
    NSuffixTree::SuffixTree tr(s);
    tr.print();
    // tr.addSymbol('a');
    // tr.addSymbol('b');
    // tr.addSymbol('c');
    // tr.addSymbol('a');
    // tr.addSymbol('$');
    // abcabxabcd
    // for(auto e : s) {
    //     tr.addSymbol(e);
    // }
    std::cout << "\n\n\n\n";
    std::cout << std::endl;

    // tr.addSymbol('$');
    // for(auto e : tr.root->nodes) {
    //     std::cout << static_cast<char>(e.first) << std::endl;
    // }

    // tr.print();
    // std::string t, p;
    // std::cin >> t >> p;
    // std::cout << t << ' ' << p << '\n';
}