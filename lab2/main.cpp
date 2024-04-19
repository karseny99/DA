#include "RBtree.hpp"
#include <iostream>


int main() {
    RBTree tr;
    std::cout << tr.add(3) << std::endl;
    std::cout << tr.add(14) << std::endl;
    std::cout << tr.add(15) << std::endl;
    std::cout << tr.add(9) << std::endl;
    std::cout << tr.add(2) << std::endl;
    std::cout << tr.add(6) << std::endl;
    std::cout << tr.add(5) << std::endl;
    std::cout << tr.add(10) << std::endl;

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

    std::cout << "===============\n";
    // std::cout << tr.remove(0) << std::endl;
    // print(&tr);
    // std::cout << "===============\n";
    // std::cout << tr.remove(10) << std::endl;
    // std::cout << tr.remove(3) << std::endl;
    // print(&tr);
    // std::cout << "===============\n";
    std::cout << tr.remove(14) << std::endl;
    print(&tr);



    // std::cout << "color " << tr.find(3)->color << std::endl;
    // std::cout << "color " << tr.find(14)->color << std::endl;
    // std::cout << "color " << tr.find(15)->color << std::endl;


    // for(int i = 0; i < 100; ++i) {
    //     std::cout << tr.add(i) << std::endl;
    // }
    // for(int i = 0; i < 100; ++i) {
    //     std::cout << tr.find(i)->color << ' ' <<  tr.find(i)->value << std::endl;
    // }

}