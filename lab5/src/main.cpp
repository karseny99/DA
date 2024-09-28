
// #include "../include/SufTree.hpp"

// #include <iostream>
// #include <memory>
// #include <string>
// #include <vector>
// #include <array>

#include <vector>
#include <iostream>
#include <string>
#include <array>
#include <map>
#include <algorithm>
#include <utility>



std::pair<bool, bool> operator+=(const std::pair<bool, bool>& x, const std::pair<bool, bool>& y) {
    return {x.first or y.first, x.second or y.second};
}

namespace NSuffixTree {

const static std::size_t ALPHABET_SIZE = 26 + 1;
const static std::size_t INF = 1e9;
const static std::size_t undefinedNode = std::size_t(-1);
const static int SHIFT = 4;



class SuffixTree {

private:


    struct SuffixNode {
        std::size_t left = 0;
        std::size_t length = 0;

        std::size_t suffixLink = 0;

        std::map<char, std::size_t> childs;
        // std::array<std::size_t, ALPHABET_SIZE> childs;
        
        SuffixNode(
                    std::size_t _l, 
                    std::size_t _len
                ) : left(_l), length(_len)
        { 
            // childs.fill(std::size_t(undefinedNode));     
        } 
    };

private:

    std::vector<SuffixNode> nodes;
    std::string text;
    std::size_t size = 0;

    std::size_t currentNode = 0;
    std::size_t reminder = 0;

    // std::size_t (char edge) const {
    //     return (edge != '$') ? (edge - 'a') : (ALPHABET_SIZE - 1);;
    // }

    std::size_t move(std::size_t node, char c) {
        if(!nodes[node].childs.contains(c)) {
            return undefinedNode;
        }
        return nodes[node].childs[c];
    }

    void move_node() {
        while(1) {
            char c = text[size - reminder];
            std::size_t nextNode = move(currentNode, c);

            if(nextNode == undefinedNode) return;
            // std::cout << c << std::endl;
            
            // std::cout << c << ' ' << nodes[nextNode].length << std::endl;
            if(nodes[nextNode].length < reminder) {
                reminder -= nodes[nextNode].length;
                // std::cout << currentNode << ' ' << nextNode << std::endl;

                currentNode = nextNode;
            } else return;
        }
    }

    std::size_t create_suffix_node(std::size_t left = 0, std::size_t length = INF) {
        nodes.push_back(SuffixNode(left, length));
        return nodes.size() - 1;
    }

public:

    SuffixTree(const std::string& s) : text(s) {
        size = 0;
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
                // std::cout << currentNode<< std::endl;

                nodes[previousNode].suffixLink = currentNode;

                previousNode = currentNode;
                
            } else {

                /* Rule 2
                    If we split an edge and insert a new node, 
                    and if that is not the first node created during the current step, 
                    we connect the previously inserted node and the new node through a special pointer, 
                    a suffix link
                */

                std::size_t goodLength = reminder - 1;
                char unmatchedSymbol = text[nodes[nextNode].left + goodLength];
                std::cout <<  nextNode << ' ' << symbol << std::endl;
               /* ??? size - 1 ======= nodes[currentNode].left + reminder - 1*/
                if(unmatchedSymbol != symbol) {
                    std::size_t internalNode = create_suffix_node(nodes[nextNode].left, goodLength);
                    std::size_t leaf = create_suffix_node(size - 1, INF);

                    nodes[nextNode].left += goodLength;
                    nodes[nextNode].length -= goodLength;
                    nodes[currentNode].childs[c] = internalNode;

                    nodes[internalNode].childs[unmatchedSymbol] = nextNode;
                    nodes[internalNode].childs[c] = leaf;
                    
                    nodes[previousNode].suffixLink = internalNode;
                    previousNode = internalNode;
                } else {
                    nodes[previousNode].suffixLink = currentNode;
                    return;
                }

            }
            if(currentNode == 0) {
                --reminder;
            } else {
                currentNode = nodes[currentNode].suffixLink;
            }
        }
    }

    // void print(std::size_t id, int height) const {
        
    //     for(auto& edge : nodes[id].childs) {
    //         // if(edge.second == undefinedNode) continue;
    //         for(int i = 0; i < SHIFT * (height - 1); ++i) {std::cout << ' ';}

    //         std::cout << "--> " << edge.second << ", ";
    //         for(int j = 0; j < std::min(size - nodes[edge.second].left, nodes[edge.second].length); ++j) {
    //             std::cout << text[nodes[edge.second].left + j];
    //         }
    //         std::cout << ' ' << nodes[edge.second].suffixLink << std::endl;
    //         print(edge.second, height + 1);
    //     }
        
    // }

    void print_edge(int v, std::ostream & out) {
        int edge_len = std::min(size - nodes[v].left, nodes[v].length);
        for (int i = 0; i < edge_len; ++i) {
            out << text[nodes[v].left + i];
        }
    }

    void print(int id, std::ostream & out, int h) {
        for (auto elem : nodes[id].childs) {
            for (int i = 0; i < SHIFT * (h - 1); ++i) {
                out << ' ';
            }
            int v = elem.second;
            out << "|-> {id = " << v << ", ";
            print_edge(v, out);
            out << ", link = " << nodes[v].suffixLink << "}\n";
            print(v, out, h + 1);
        }
    }

    friend std::ostream & operator << (std::ostream & out, SuffixTree & st) {
        out << "root\n";
        st.print(0, out, 1);
        return out;
    }

    // int get_longest_common_substring(const int& firstLength) {
    //     std::map<int, std::pair<bool, bool>> mp;
    //     std::vector<bool> visited(nodes.size(), false);

    //     dfs(0, visited, mp, firstLength);
        
    //     for(int i = 0; i < nodes.size(); ++i) {
    //         std::cout << i << ' ' << (mp[i].first == true and mp[i].second == true) << std::endl; 
    //     }

    //     return 0;
    // }

    // std::pair<bool, bool> dfs(std::size_t cur, std::vector<bool>& visited, std::map<int, std::pair<bool, bool>>& mp, const int& firstLength) {
    //     if(visited[cur]) return mp[cur];

    //     visited[cur] = true;

    //     for(std::pair<char, std::size_t> next : nodes[cur].childs) {
    //         mp[cur] += dfs(next.second, visited, mp, firstLength);
    //     }

    //     if(nodes[cur].childs.empty()) { // Leaf
    //         if(nodes[cur].left < firstLength) { // long suffix
    //             mp[cur] = std::make_pair(true, false);
    //         } else if(nodes[cur].left > 1 + firstLength) { // short suffix
    //             mp[cur] = std::make_pair(false, true);
    //         }
    //     }

    //     return mp[cur];
    // }
};
}




int main() {
    // std::ios::sync_with_stdio(false);
    // std::cin.tie(0);

    std::string s = "xabxa!aab$";
    NSuffixTree::SuffixTree tr(s);
    // tr.print(0, 1);
    // std::cout << tr << std::endl;
    // tr.get_longest_common_substring(5);
    // std::pair<bool, bool> f = {false, true};
    // std::pair<bool, bool> s = {true, false};
    // std::pair<bool, bool> t = s + f;

    // std::cout << t.first << ' ' << t.second << std::endl;

    // tr.addSymbol('a');
    // tr.addSymbol('b');
    // tr.addSymbol('c');
    // tr.addSymbol('a');
    // tr.addSymbol('$');
    // abcabxabcd
    // for(auto e : s) {
    //     tr.addSymbol(e);
    // }
    // std::cout << "\n\n\n\n";
    // std::cout << std::endl;

    // tr.addSymbol('$');
    // for(auto e : tr.root->nodes) {
    //     std::cout << static_cast<char>(e.first) << std::endl;
    // }

    // tr.print();
    // std::string t, p;
    // std::cin >> t >> p;
    // std::cout << t << ' ' << p << '\n';
}