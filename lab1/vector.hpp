#pragma once

#include <iostream>

const int min_capacity = 5;

template<class T>
class Vector {
    private:
        int size;
        int capacity;

        T* buffer = nullptr;

        bool try_increase_buffer();

        bool try_decrease_buffer();
    
    public:

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

        class iterator;

        iterator begin();

        iterator end();
};

template<class T> 
class Vector<T>::iterator {
    private:
        T* cur;
    public:
        iterator(T* p) : cur(p) {}

        iterator& operator++() {
            ++cur;
            return *this;
        }

        iterator& operator--() {
            --cur;
            return *this;
        }

        T& operator*()
        {
            return *cur;
        }

        bool operator==(const iterator& b) const
        {
            return *cur == *b.cur;
        }

        bool operator!=(const iterator& b) const
        {
            return *cur != *b.cur;
        }
};


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

template<class T>
inline typename Vector<T>::iterator Vector<T>::begin()
{	
	return Vector<T>::iterator(&buffer[0]);
}

template<class T>
inline typename Vector<T>::iterator Vector<T>::end()
{	
	return Vector<T>::iterator(&buffer[size]);
}