#pragma once
#include <string>


const int array_digit_size = 16;
const int key_length = 32;


class TValue {   
public:


    char key[key_length];
    uint64_t value; 

    TValue() = default;
    ~TValue() = default;

    TValue(std::string& _key, uint64_t _value) {
        for(int i = 0; i < key_length; ++i) 
            key[i] = _key[i];

        value = _value;
    }

    void set(std::string& _key, uint64_t _value) {

        for(int i = 0; i < key_length; ++i) 
            key[i] = _key[i];

        value = _value;
    }
};