#include <iostream>
#include <cstdint>
#include <string>
#include <iostream>



const int min_capacity = 5;
const int MAX_ARRAY_SIZE = 1e6;

namespace vector {

    template <class T>
    class Vector {
    private:

        int size;
        int capacity;
        T* buffer = nullptr;

        bool is_lazy = false;

        bool try_increase_buffer();

        bool try_decrease_buffer();


    public:

        class iterator;
        // class const_iterator;

        template<class Type>
        friend void swap(Vector<Type>& A, Vector<Type>& B);

        //Beginning of iterator
        class iterator {
        private:
            Vector<T>* vector;
            T* current;

        public:

            iterator() :
                vector(nullptr), current(nullptr) {}

            iterator(Vector<T>* vectorPtr, T* current):
                vector(vectorPtr), current(current) {}

            iterator(Vector<T>::iterator& sIterator): 
                vector(sIterator.vector), current(sIterator.current) {
            }


            iterator& operator=(const iterator& sIterator);
            iterator& operator++(); 
            iterator& operator--(); 
            const T operator*() const;

            friend class Vector;
        };


        Vector() : capacity(min_capacity), size(0) {
            buffer = new T[capacity];
        }

        Vector(bool _is_lazy) : capacity(min_capacity), size(0), is_lazy(_is_lazy) {
            buffer = new T[capacity];
        }

        ~Vector() {
            delete[] buffer;
            buffer = nullptr;
            size = 0;
            capacity = 0;
            is_lazy = false;
        }

        Vector(int _size) : size(_size) {
            if(size <= 0) 
                throw std::runtime_error("Error: Wrong given size");
            capacity = size * 2;
            buffer = new T[capacity];
        } 

        Vector(int _size, T element) : size(_size) {
            if(size <= 0) 
                throw std::runtime_error("Error: Wrong given size");

            capacity = size * 2;
            buffer = new T[capacity];

            for(int i = 0; i < capacity; ++i) 
                buffer[i] = element;
        } 

        Vector(int _size, bool _is_lazy) : size(_size), is_lazy(_is_lazy) {
            if(size <= 0) 
                throw std::runtime_error("Error: Wrong given size");
            capacity = size * 2;
            buffer = new T[capacity];
        } 

        Vector(int _size, T element, bool _is_lazy) : size(_size), is_lazy(_is_lazy) {
            if(size <= 0) 
                throw std::runtime_error("Error: Wrong given size");

            capacity = size * 2;
            buffer = new T[capacity];

            for(int i = 0; i < capacity; ++i) 
                buffer[i] = element;
        } 



        // Vector::const_iterator begin() const;
        // Vector::const_iterator end() const;
        Vector::iterator begin();
        Vector::iterator end();
        void push_back(T element);

        void pop_back();

        T& operator[](const int& idx) {
            return buffer[idx];
        } 

        const T& operator[](const int& idx) const {
            return buffer[idx];
        }

        int empty() const {
            return size == 0;
        }

        int get_size() const {
            return size;
        }

        int get_capacity() const {
            return capacity;
        }

    };

    template <class T>
    typename Vector<T>::iterator Vector<T>::begin() {
        Vector::iterator iterator;
        iterator.vector = this;
        iterator.current = this->buffer;
        return iterator;
    }

    template<class T>
    typename Vector<T>::iterator Vector<T>::end() {
        Vector::iterator iterator;
        iterator.vector = this;
        iterator.current = &(this->buffer[this->size]);
        return iterator;
    }

    template <class T>
    typename Vector<T>::iterator& Vector<T>::iterator::operator=(const Vector<T>::iterator& sIterator) {
        current = sIterator.current;
        vector = sIterator.vector;
    }

    template <class T>
    typename Vector<T>::iterator& Vector<T>::iterator::operator++() {
        ++current;
        return *this;
    }

    template <class T>
    typename Vector<T>::iterator& Vector<T>::iterator::operator--() {
        --current;
        return *this;
    }


    template<class T>
    inline bool Vector<T>::try_increase_buffer() {
        
        if(is_lazy) {
            if(capacity > size ) 
                return true;
        } else {
            if(capacity > size * 2)
                return true;
        }

        int new_capacity = std::min(capacity * 2, MAX_ARRAY_SIZE);
        T* tmp_buffer = new T[new_capacity];

        if(tmp_buffer == nullptr)
            return false;

        for(int i = 0; i < size; ++i) {
            tmp_buffer[i] = buffer[i];
        }

        delete[] buffer;
        buffer = tmp_buffer;
        capacity = new_capacity;
        return true;
    }

    template<class T>
    inline bool Vector<T>::try_decrease_buffer() {
        if(capacity < size * 4) 
            return true;
        
        int new_capacity = std::min(capacity / 2, min_capacity);
        T* tmp_buffer = new T[new_capacity];

        if(tmp_buffer == nullptr) 
            return false;
        
        for(int i = 0; i < size; ++i) 
            tmp_buffer[i] = buffer[i];
        
        delete[] buffer;
        buffer = tmp_buffer;
        capacity = new_capacity;
        return false;
    }

    template<class T>
    inline void Vector<T>::push_back(T element) {
        if(!try_increase_buffer()) 
            throw std::runtime_error("Error: Can't increase buffer");

        buffer[size++] = element;       
    }

    template<class T>
    inline void Vector<T>::pop_back() {
        if(!try_decrease_buffer()) 
            throw std::runtime_error("Error: Can't decrease buffer");
        
        ~buffer[size--];
    }

    template <class T>
    void swap(Vector<T>& A, Vector<T>& B) {
        T* tmp_pointer = A.buffer;
        A.buffer = B.buffer;
        B.buffer = tmp_pointer;
    }

}





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

namespace sort {

    const int array_digit_size = 16;
    const int key_length = 32;

    void radix_sort( vector::Vector<TValue>& elems ) {

        vector::Vector<TValue> tmpResult(elems.get_size() + 1, true);

        for(int j = key_length - 1; j >= 0; --j) {

            int tmp[array_digit_size];
            for(int k = 0; k < array_digit_size; ++k) {
                tmp[k] = 0;
            }

            for(int i = 0; i < elems.get_size(); ++i) {
                if('0' <= elems[i].key[j] and elems[i].key[j] <= '9')
                    ++tmp[elems[i].key[j] - '0'];
                else 
                    ++tmp[elems[i].key[j] - 'a' + 10];
            }

            for(int k = 1; k < array_digit_size; ++k) {
                tmp[k] += tmp[k - 1];
            }

            for(int i = elems.get_size() - 1; i >= 0; --i) {
                int key;
                if('0' <= elems[i].key[j] and elems[i].key[j] <= '9')
                    key = elems[i].key[j] - '0';
                else    
                    key = elems[i].key[j] - 'a' + 10;

                int pos = tmp[key]--;
                tmpResult[pos-1] = elems[i];
            }

            vector::swap(elems, tmpResult);
        }

    }

}


int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(0); std::cout.tie(0);

    std::string key;
    uint64_t value;

    vector::Vector<TValue> elems(true);

    while(std::cin >> key >> value) {
        elems.push_back(TValue(key, value));
    }

    sort::radix_sort( elems );

    for (int i = 0 ; i < elems.get_size(); ++i) {
        for(int j = 0; j < key_length; ++j) 
            std::cout << elems[i].key[j];
        std::cout << '\t' << elems[i].value << '\n';
    }
}
