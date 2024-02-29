#pragma once

#include <iostream>

const int min_capacity = 5;

// exception definitions
class VectorpingDoesntExist : public std::exception {
public:
	virtual const char* what() const throw() {
		return "Vectorping for key was not found!\n";
	}
};
class VectorpingAlreadyExists : public std::exception {
public:
	virtual const char* what() const throw() {
		return "Key already exists";
	}
};


template <class T>
class Vector {
private:

    int size;
    int capacity;


    bool try_increase_buffer();

    bool try_decrease_buffer();


public:

    T* buffer = nullptr;
	/* 
	Class declarations
	*/
	class iterator;
	class const_iterator;


	//Beginning of iterator
	class iterator {
	private:
		int idx;
        int size;
		Vector<T>* vector;

	public:

		iterator() :
			idx(0), vector(NULL), size(0) {}

		iterator(Vector<T>* vectorPtr, int _idx, int _size):
			idx(_idx), vector(vectorPtr), size(_size) {}

		iterator(Vector<T>::iterator& sIterator): 
			idx(sIterator.idx),	vector(sIterator.vector), size(sIterator.size) {
		}


		iterator& operator=(const iterator& sIterator);
		iterator& operator++(); 
        iterator& operator--(); 
		iterator& operator-(const Vector<T>::iterator& sIterator); 
		bool operator==(const iterator& sIterator) const;
		bool operator!=(const iterator& sIterator) const;
		const T operator*() const;

		friend class Vector;
	};


	class const_iterator {
	private:
		int idx;
        int size;
		const Vector<T>* vector;

	public:

		const_iterator() :
			idx(0), vector(NULL), size(0) {}

		const_iterator(const Vector<T>* VectorPtr, int _idx, int _size):
			idx(_idx), vector(VectorPtr), size(_size) {}

		const_iterator(const Vector<T>::const_iterator& sIterator): 
			idx(sIterator.idx),	vector(sIterator.vector), size(sIterator.size) {}

		const_iterator(const Vector<T>::iterator& sIterator):
			idx(sIterator.idx), vector(sIterator.vector), size(sIterator.size) {}

		
		const_iterator& operator=(const const_iterator& sIterator);
		const_iterator& operator++(); 
		const_iterator& operator--(); 
		bool operator==(const const_iterator& sIterator) const;
		bool operator!=(const const_iterator& sIterator) const;
		const T operator*() const;

		friend class Vector;
	};


    Vector() : capacity(min_capacity), size(0) {
        buffer = new T[capacity];
    }

    ~Vector() {
        delete[] buffer;
        buffer = nullptr;
        size = 0;
        capacity = 0;
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


	Vector::const_iterator begin() const;
	Vector::const_iterator end() const;
	Vector::iterator begin();
	Vector::iterator end();
    void push_back(T element);

    void pop_back();

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

};

/*
Vector methods and operators
*/ 
template<class T>
typename Vector<T>::const_iterator Vector<T>::begin() const {
	Vector::const_iterator iterator;
	iterator.vector = this;
    iterator.idx = 0;
    iterator.size = this->size;
	return iterator;
}

template<class T>
typename Vector<T>::const_iterator Vector<T>::end() const {
	Vector::const_iterator iterator;
	iterator.vector = this;
    iterator.idx = size;
    iterator.size = this->size;
	return iterator;
}

template <class T>
typename Vector<T>::iterator Vector<T>::begin() {
	Vector::iterator iterator;
	iterator.vector = this;
    iterator.idx = 0;
    iterator.size = this->size;
	return iterator;
}

template<class T>
typename Vector<T>::iterator Vector<T>::end() {
	Vector::iterator iterator;
	iterator.vector = this;
    iterator.idx = size;
    iterator.size = this->size;
	return iterator;
}

template <class T>
typename Vector<T>::iterator& Vector<T>::iterator::operator=(const Vector<T>::iterator& sIterator) {
	idx = sIterator.idx;
    size = sIterator.size;
	vector = sIterator.vector;
    return *this;
}

template <class T>
typename Vector<T>::iterator& Vector<T>::iterator::operator++() {
	++idx;
	return *this;
}

template <class T>
typename Vector<T>::iterator& Vector<T>::iterator::operator--() {
	--idx;
	return *this;
}

template <class T>
typename Vector<T>::iterator& Vector<T>::iterator::operator-(const Vector<T>::iterator& sIterator) {
	idx -= sIterator.idx;
	return *this;
}

template <class T>
bool Vector<T>::iterator::operator==(const Vector<T>::iterator& sIterator) const {
	return (idx == sIterator.idx) and (vector == sIterator.vector) and (size == sIterator.size);
}

template <class T>
bool Vector<T>::iterator::operator!=(const Vector<T>::iterator& sIterator) const {
	return !(idx == sIterator.idx) or !(vector == sIterator.vector) or !(size == sIterator.size);
}

template <class T>
const T Vector<T>::iterator::operator*() const {
	return vector->buffer[idx];
}

template <class T>
typename Vector<T>::const_iterator& Vector<T>::const_iterator::operator=(const Vector<T>::const_iterator& sIterator) {
	idx = sIterator.idx;
	vector = sIterator.vector;
	size = sIterator.size;
}

template <class T>
typename Vector<T>::const_iterator& Vector<T>::const_iterator::operator++() {
	++idx;
	return *this;
}
template <class T>
typename Vector<T>::const_iterator& Vector<T>::const_iterator::operator--() {
	--idx;
	return *this;
}

template <class T>
bool Vector<T>::const_iterator::operator==(const Vector<T>::const_iterator& sIterator) const {
	return (idx == sIterator.idx) and (vector == sIterator.vector) and (size == sIterator.size);
}

template <class T>
bool Vector<T>::const_iterator::operator!=(const Vector<T>::const_iterator& sIterator) const {
	return !(idx == sIterator.idx) or !(vector == sIterator.vector) or !(size == sIterator.size);
}

template <class T>
const T Vector<T>::const_iterator::operator*() const {
	return vector->buffer[idx];
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


