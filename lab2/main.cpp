#include "RBtree2.hpp"
#include <iostream>
#include <vector>

std::vector<long long> seq = {3, 14, 15, 9, 2, 6, 5, 10};

int main() {
    RBTree tr;

    for(auto e : seq) {
        tr.insert(e);
    }
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
    // for(auto e : seq) {
    //     std::cout << "===============" << e << std::endl;
    //     tr.erase(e);
    //     print(&tr);
    // }
    std::cout << tr.erase(14) << std::endl;
    std::cout << "===============\n";
    print(&tr);

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