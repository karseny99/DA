#include <iostream>
#include <cstdint>


const int elemsray_digit_size = 16;
const int key_length = 32;

// using Pair = std::pair<chelems[key_length], uint64_t>;

class TValue{
public:
    char key[key_length];
    uint64_t value; 
    void set(std::string& _key, uint64_t _value) {
        for(int i = 0; i < key_length; ++i) 
            key[i] = _key[i];
        value = _value;
    }
};


void tmping_sort( TValue* elems, TValue* tmpResult, int size, int j )
{

    int tmp[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    for (int i = 0; i < size; ++i) {
        if('0' <= elems[i].key[j] and elems[i].key[j] <= '9')
            ++tmp[elems[i].key[j] - '0'];
        else 
            ++tmp[elems[i].key[j] - 'a' + 10];
    }
   

    for (int i = 1; i < elemsray_digit_size; ++i) {
        tmp[i] += tmp[i-1];
    } 

    for (int i = size - 1; i >= 0; --i ) {
        int key;
        if('0' <= elems[i].key[j] and elems[i].key[j] <= '9')
            key = elems[i].key[j] - '0';
        else    
            key = elems[i].key[j] - 'a' + 10;

        int pos = tmp[key]--;
        tmpResult[pos-1] = elems[i];
        // std::cout <<pos - 1 << ' ' << j << std::endl;
    }

    // for(int i = 0 ; i < size; ++i) {
    //     std::cout << tmpResult[i].key << std::endl;
    // }

    // std::cout << "===========================" << std::endl;
    // std::cout << j << std::endl;
    // std::cout << "===========================" << std::endl;

    TValue *tmp1 = elems;
    elems = tmpResult;
    tmpResult = tmp1;
}



void radix_sort(TValue *elems, int size) 
{
    // 32 * 4 = 128 >> 64
    TValue *tmpResult = new TValue[size];

    for(int j = key_length - 1; j >= 0; --j)
    {
        // tmpSort
        int tmp[16];
        for(int k = 0; k < 16; ++k) 
        {
            tmp[k] = 0;
        }

        for(int i = 0; i < size; ++i) 
        {
            if('0' <= elems[i].key[j] and elems[i].key[j] <= '9')
                ++tmp[elems[i].key[j] - '0'];
            else 
                ++tmp[elems[i].key[j] - 'a' + 10];
        }

        for(int k = 1; k < 16; ++k) {
            tmp[k] += tmp[k - 1];
        }

        for(int i = size - 1; i >= 0; --i) 
        {
            int key;
            if('0' <= elems[i].key[j] and elems[i].key[j] <= '9')
                key = elems[i].key[j] - '0';
            else    
                key = elems[i].key[j] - 'a' + 10;
            int pos = tmp[key]--;
            tmpResult[pos-1] = elems[i];
        }

        TValue *tmp1 = elems;
        elems = tmpResult;
        tmpResult = tmp1;
    }

    delete[] tmpResult;

}



int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(0); std::cout.tie(0);


    std::string key;
    uint64_t value;

    TValue* elems = new TValue[(int)1e6];
    int size = 0;

    while (std::cin >> key >> value)
    {
        elems[size].set(key, value);
        ++size;
    }

    radix_sort( elems, size );

    for (int i = 0 ; i < size ; ++i)
    {
        for(int j = 0; j < key_length; ++j) 
            std::cout << elems[i].key[j];
        std::cout << ' ' << elems[i].value << std::endl;
    }

    delete[] elems; 

    return 0;
}
