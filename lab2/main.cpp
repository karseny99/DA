#include "map.hpp"
#include "RBTree.hpp"
#include <iostream>
#include <vector>
#include <fstream>

std::vector<NMap::TPair<int, std::string>> seq = {{3, "adas"}, {14, "abc"}, {15, "da"}, {9, "da"}, {2, "ty"}, {6, "lk"}, {5, "jh"}, {10, "fgh"}};

int main() {
    // mTMap::TPair<int, std::string> p(3, c);
    // mTMap::TPair<int, std::string> TPair = {3, "dsafds"};
    
    NMap::TRBTree<NMap::TPair<int, std::string>> tr;

    for(auto e : seq) {
        tr.insert(e);
    }
    NMap::print(&tr);

    // for(auto e : seq) {
    //     std::cout << "===============" << e.first << std::endl;
    //     tr.erase(e.first);
    //     print(&tr);
    // }
    NMap::TMap<int, std::string> mp;
    for(auto e : seq) {
        std::cout << "===============" << e.first << std::endl;
        mp[e.first] = e.second;
    }
    for(auto e : seq) {
        std::cout << "===============" << e.first << std::endl;
        std::cout << mp[e.first] << std::endl;
    }

    // std::ofstream os("1.txt", std::ios::binary);
    // mp.save_to_file(os);
    // os.close();
    std::cout << std::endl << std::endl << std::endl;
    std::ifstream is("1.txt", std::ios::binary);
    mp.load_from_file(is);
    // std::cout << mp.insert({3, "fdsdfs"}) << std::endl;
    // mp[0] = "3333333";
    // std::cout << mp[0] << std::endl;
    // std::cout << mp.erase(0) << std::endl;
    // std::cout << tr.insert(3) << std::endl;
    // std::cout << tr.insert(14) << std::endl;
    // std::cout << tr.insert(15) << std::endl;
    // std::cout << tr.insert(9) << std::endl;
    // std::cout << tr.insert(2) << std::endl;
    // std::cout << tr.insert(6) << std::endl;
    // std::cout << tr.insert(5) << std::endl;
    // std::cout << tr.insert(10) << std::endl;
    // std::cout << tr.insert(10) << std::endl;
    // std::cout << "===============\n";
    // print(&tr);
    // std::cout << "===============\n";

    // for(int i = 0; i < 100; ++i)
    //     std::cout << tr.insert(i) << std::endl;
    // print(&tr);
    // std::cout << "===============\n";
    // tr.erase(3);
    // print(&tr);
    // std::cout << "===============\n";
    // std::cout << tr.add(3) << std::endl;
    // std::cout << tr.add(14) << std::endl;
    // std::cout << tr.add(15) << std::endl;
    // std::cout << tr.add(9) << std::endl;
    // std::cout << tr.add(2) << std::endl;
    // std::cout << tr.add(6) << std::endl;
    // std::cout << tr.add(5) << std::endl;
    // std::cout << tr.add(10) << std::endl;
    // std::cout << tr.erase(14) << std::endl;
    // std::cout << "===============\n";
    // print(&tr);

    // std::cout << tr.find(5)->parent->value << std::endl;
    // std::cout << tr.find(3)->color << ' ' <<  tr.find(3)->value << std::endl;
    // std::cout << tr.find(14)->color<< ' ' <<  tr.find(14)->value << std::endl;
    // std::cout << tr.find(15)->color<< ' ' <<  tr.find(15)->value << std::endl;
    // std::cout << tr.find(9)->color << ' ' <<  tr.find(9)->value<< std::endl;
    // std::cout << tr.find(2)->color << ' ' <<  tr.find(2)->value<< std::endl;
    // std::cout << tr.find(6)->color << ' ' <<  tr.find(6)->value<< std::endl;
    // std::cout << tr.find(5)->color << ' ' <<  tr.find(5)->value<< std::endl;
    // std::cout << tr.find(10)->color << ' ' <<  tr.find(10)->value << std::endl;

    // std::cout << "===============\n";
    // std::cout << "===============\n";
    // print(&tr);
    // std::cout << tr.remove(0) << std::endl;
    // print(&tr);
    // // std::cout << "===============\n";
    // // std::cout << tr.remove(10) << std::endl;
    // // std::cout << tr.remove(3) << std::endl;
    // // print(&tr);
    // // std::cout << "===============\n";



    // std::cout << "color " << tr.find(3)->color << std::endl;
    // std::cout << "color " << tr.find(14)->color << std::endl;
    // std::cout << "color " << tr.find(15)->color << std::endl;


    // for(int i = 0; i < 100; ++i) {
    //     tr.erase(i);
    //     print(&tr);
    //     std::cout << "=============" << i << std::endl;
    // }
    // for(int i = 0; i < 100; ++i) {
    //     std::cout << tr.find(i)->color << ' ' <<  tr.find(i)->value << std::endl;
    // }

}