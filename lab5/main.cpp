#include <vector>
#include <iostream>
#include <string>
#include <map>
#include <algorithm>


namespace NSuffixTree {

const static std::size_t INF = 1e9;
const static std::size_t undefinedNode = std::size_t(-1);

const static int SHIFT = 4;

const static int SHORT_SUFFIX = 1;
const static int LONG_SUFFIX = 2;
const static int LONG_SHORT_SUFFIX = 3;

class SuffixTree {

private:

    struct SuffixNode {
        std::size_t left = 0;
        std::size_t length = 0;

        std::size_t suffixLink = 0;

        std::map<char, std::size_t> childs;
       
        SuffixNode(
                    std::size_t _l, 
                    std::size_t _len
                ) : left(_l), length(_len) { } 
    };


private:

    std::vector<SuffixNode> nodes;
    std::string text;
    std::size_t size = 0;

    std::size_t currentNode = 0;
    std::size_t reminder = 0;

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

            if(nodes[nextNode].length < reminder) {
                reminder -= nodes[nextNode].length;
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
        create_suffix_node(0, 0);
        for(char c : text) {
            add_symbol(c);
        }
    }

    void add_symbol(char symbol) {

        /* About suffix links
            На i-ом этапе мы будем устанавливать суффиксную ссылку для внутренней вершины, 
            созданной i - 1-ом. 
            Если мы создаём новую внутреннюю вершину, 
            то суффиксная ссылка будет вести в неё.
            В двух остальных случаях суффиксная ссылка ведёт в currentNode
        */

        ++reminder;
        ++size;
        std::size_t previousNode = 0;

        while(reminder > 0) {
            /* Step 1.
                Если pos = 0, то все суффиксы обработаны. 
                Завершаемся. Иначе найдём вершину, 
                после которой находится новый суффикс. 
                То есть, пока pos больше длины ребра, 
                исходящего из данной вершины, 
                будем идти по ребру и вычитать его длину из pos.
            */
            move_node();
            char c = text[size - reminder];
            std::size_t nextNode = move(currentNode, c);

            // Step 2.

            if(nextNode == undefinedNode) {

                /* Step 2.1
                    Если у нас нет исходящего ребра по интересующему нас символу, 
                    то мы просто создаём новую вершину и подвешиваем её к текущей.
                */

                nodes[currentNode].childs[c] = create_suffix_node(size - reminder);
                nodes[previousNode].suffixLink = currentNode;
                previousNode = currentNode;
                
            } else {
                char matchingSymbol = text[nodes[nextNode].left + reminder - 1];

                /* Step 2.2
                    Если ребро есть и суффикс не лежит на нём целиком, 
                    это значит, 
                    что нам нужно создать новую вершину посередине этого ребра, 
                    к которой подвесить старую вершину с конца ребра и новую вершину, 
                    соответствующую суффиксу. 
                    Стоит заметить, что ребро к новому листу в данный момент будет иметь длину, 
                    равную единице.
                */
                if(matchingSymbol != symbol) {
                    std::size_t internalNode = create_suffix_node(nodes[nextNode].left, reminder - 1);
                    std::size_t leaf = create_suffix_node(size - 1, INF);

                    nodes[nextNode].left += reminder - 1;
                    nodes[nextNode].length -= reminder - 1;

                    nodes[currentNode].childs[c] = internalNode;

                    nodes[internalNode].childs[matchingSymbol] = nextNode;
                    nodes[internalNode].childs[symbol] = leaf;
                    
                    nodes[previousNode].suffixLink = internalNode;
                    previousNode = internalNode;
                } else {
                    /* Step 2.3
                        Если ребро есть и суффикс, 
                        который мы хотим добавить целиком лежит на нём, 
                        завершаем свою работу — этот и дальнейшие суффиксы не являются уникальными.
                    */
                    nodes[previousNode].suffixLink = currentNode;
                    return;
                }

            }
            /*
            Step 3
                Если мы не завершили работу на прошлом шаге, 
                переходим к следующему суффиксу. 
                Если мы находимся в корне, 
                то мы уменьшаем pos на единицу,
                иначе же мы просто переходим по суффиксной ссылке node = link(node). 
                После этого мы переходим к пункту 1.
            */
            if(currentNode == 0) {
                --reminder;
            } else {
                currentNode = nodes[currentNode].suffixLink;
            }
        }
    }


    std::vector<int> suffixTypes;
    std::vector<std::pair<std::size_t, std::size_t>> entries; 

    void get_longest_common_substring(std::size_t firstLength) {
        suffixTypes.resize(nodes.size(), 0);
        get_suffix_types(0, 0, firstLength);

        find_common_substrings(0, 0);
        if(entries.size() == 0) {
            std::cout << "0\n";
            return;
        }

        std::size_t maxLen = (*std::max_element(entries.begin(), entries.end(), 
            [](const std::pair<std::size_t, std::size_t>& fst, const std::pair<std::size_t, std::size_t>& scd) 
                { return fst.second < scd.second; } )).second;

        std::cout << maxLen << '\n';
        for(const std::pair<std::size_t, std::size_t>& entry : entries) {
            if(entry.second == maxLen) {
                for(std::size_t i{0}; i < entry.second; ++i) 
                    std::cout << text[i + entry.first];
                std::cout << '\n';
            }
        }

    }
    
    void find_common_substrings(std::size_t cur, std::size_t localLength) {
        bool hasExpands = false;
        for(std::pair<char, std::size_t> next : nodes[cur].childs) {
            if(suffixTypes[next.second] == LONG_SHORT_SUFFIX) {
                hasExpands = true;
                find_common_substrings(next.second, localLength + nodes[cur].length);
            }
        }

        if(!hasExpands) {
            entries.emplace_back(std::make_pair(nodes[cur].left - localLength, localLength + nodes[cur].length));
        }
    }

    int get_suffix_types(std::size_t cur, std::size_t localLength, std::size_t firstLength) {
        
        for(std::pair<char, std::size_t> next : nodes[cur].childs) {
            suffixTypes[cur] |= get_suffix_types(next.second, localLength + nodes[cur].length, firstLength);
        }

        if(nodes[cur].childs.empty()) { // Leaf
            if(nodes[cur].left - localLength < firstLength) {  
                suffixTypes[cur] = LONG_SUFFIX;
            } else {
                suffixTypes[cur] = SHORT_SUFFIX;
            }
        }

        return suffixTypes[cur];
    }
};

}




int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    
    std::string s1, s2;
    std::cin >> s1 >> s2;
    std::string s = s1 + '!' + s2 + '$'; 
    NSuffixTree::SuffixTree tr(s);
    tr.get_longest_common_substring(s1.size());
}