#pragma once

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


