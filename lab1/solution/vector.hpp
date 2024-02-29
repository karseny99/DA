#pragma once

#include <iostream>

const int min_capacity = 5;

// template <class ItemType, class ArrayType>
// class ArrayIterator
// {
// private:
//     ArrayType *array;
//     int index;
//     int size;


// public:
//     ArrayIterator(ArrayType *value, int i, int s) : array(value), index(i), size(s){};

//     ItemType& operator*()
//     {
//         if (index >= size)
//                 throw std::logic_error("Error: wrong index " + std::to_string(index) + " in vector of size " + std::to_string(size));
//         return (*array)[index];
//     }

//     ItemType* operator->()
//     {
//         if (index >= size)
//                 throw std::logic_error("Error: wrong index " + std::to_string(index) + " in vector of size " + std::to_string(size));
//         return (*array)[index];
//     }

//     bool operator!=(ArrayIterator<ItemType, ArrayType> const &other) const
//     {
//         return (other.index != index) || (other.array != array);
//     }

//     bool operator==(ArrayIterator<ItemType, ArrayType> const &other) const
//     {
//         return (other.index == index) && (other.array == array);
//     }
    
//     ArrayIterator<ItemType, ArrayType> &operator++()
//     {
//         ++index;
//         return *this;
//     }

// };

template<class T>
class Vector {
    private:
        int size;
        int capacity;

        T* buffer = nullptr;

        bool try_increase_buffer();

        bool try_decrease_buffer();
    
    public:

        class iteratir;

        ~Vector() {
            delete[] buffer;
            buffer = nullptr;
            size = 0;
            capacity = 0;
        }

        Vector() : capacity(min_capacity), size(0) {
            buffer = new T[capacity];
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

        void push_back(T element);

        void pop_back();

        // T top() {
        //     return buffer[(head) % capacity];
        // }

        // void pop_front() {
        //     ~buffer[(head)];
        //     ++head;
        //     --size;
        //     if(!try_decrease_buffer()) 
        //         throw std::runtime_error("Error: Can't decrease buffer");
        // }

        T& operator[](const int& idx) {
            if(idx >= size) 
                throw std::logic_error("Error: wrong index " + std::to_string(idx) + " in vector of size " + std::to_string(size));
            return buffer[idx];
        } 

        const T& operator[](const int& idx) const {
            if(idx >= size) 
                throw std::logic_error("Error: wrong index " + std::to_string(idx) + " in vector of size " + std::to_string(size));
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

        class iterator {
            private:
                T *current;
                Vector<T>* vector;
                int index = 0;
                int size = 0;
            public:
                iterator() : current(NULL), index(0), size(0) {}
                iterator(Vector<T>* vectorPtr) : vector(vectorPtr) {
                    T* current = &vector->buffer[index];
                }
                // iterator& operator=(const iterator& sIterator);
                // iterator& operator++(); 
                // iterator operator++(int);
                // bool operator==(const iterator& sIterator) const;
                // bool operator!=(const iterator& sIterator) const;
                // const Pair<KeyType,DataType> operator*() const;
                // const KeyType& getKey() const;
                // DataType& getData();
        
                friend class Vector;
        }

        Vector::iterator begin();
        Vector::iterator end();
};

template <class T>
typename Vector<T>::iterator Vector<T>::begin() {
    Vector::iterator iterator;
    iterator.current = head;
    iterator.map = this;
    return iterator;
}
 
template <class T>
typename Vector<T>::iterator Vector<T>::end() {
    return Vector<T>::iterator();
}

template<class T>
inline bool Vector<T>::try_increase_buffer() {
    if(capacity > size * 2)
        return true;

    int new_capacity = capacity * 2;
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

