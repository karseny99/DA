#include <iostream>
#include <cstdint>


using Pair = std::pair<std::string, uint64_t>;

const int min_capacity = 5;

template <class T>
class Vector {
private:

    int size;
    int capacity;
    T* buffer = nullptr;

    bool isStatic = false;

    bool try_increase_buffer();

    bool try_decrease_buffer();


public:

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

    Vector(int _size, bool _isStatic) : size(_size), isStatic(_isStatic) {
        if(size <= 0) 
            throw std::runtime_error("Error: Wrong given size");
        capacity = isStatic ? size : size * 2;
        buffer = new T[capacity];
    } 

    Vector(int _size, const T& element, bool _isStatic) : size(_size), isStatic(_isStatic) {
        if(size <= 0) 
            throw std::runtime_error("Error: Wrong given size");

        capacity = isStatic ? size : size * 2;
        buffer = new T[capacity];

        for(int i = 0; i < capacity; ++i) 
            buffer[i] = element;
    } 
    
    Vector(int _size, const T& element) : size(_size) {
        if(size <= 0) 
            throw std::runtime_error("Error: Wrong given size");

        capacity = size * 2;
        buffer = new T[capacity];

        for(int i = 0; i < capacity; ++i) 
            buffer[i] = element;
    } 

    Vector(bool _isStatic) : isStatic(_isStatic) {
        capacity = min_capacity;
        buffer = new T[capacity];
        size = 0;
    }

	Vector::const_iterator begin() const;
	Vector::const_iterator end() const;
	Vector::iterator begin();
	Vector::iterator end();
    
    void push_back(const T& element);

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

    bool empty() const {
        return size == 0;
    }

    int get_size() const {
        return size;
    }

    int get_capacity() const {
        return capacity;
    }

    void reserve(const int& cap) {
        delete[] buffer;
        capacity = cap;
        buffer = new T[capacity];
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
    if(capacity > size or isStatic)
        return true;
    
    int new_capacity = std::min(capacity * 2, 1500000);
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
    if(capacity < size * 4 or isStatic) 
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
inline void Vector<T>::push_back(const T& element) {
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









const int array_digit_size = 16;
const int key_length = 32;

void counting_sort( Vector<Pair>& elems, Vector<Pair>& tmpResult, int idx )
{

    if(idx < 0) 
        throw std::logic_error("Error: wrong index " + idx);

    if (elems.empty())
    {
        return;
    } 


    Vector<int> tmp(array_digit_size, 0, true);
    for (int i = 0; i < elems.get_size(); ++i) {
        if('0' <= elems[i].first[idx] and elems[i].first[idx] <= '9')
            ++tmp[elems[i].first[idx] - '0'];
        else 
            ++tmp[elems[i].first[idx] - 'a' + 10];
    }

    for (int i = 1; i < tmp.get_size(); ++i) {
        tmp[i] += tmp[i-1];
    } 
    
    for (int i = elems.get_size() - 1; i >= 0; --i ) {
        int key;
        if('0' <= elems[i].first[idx] and elems[i].first[idx] <= '9')
            key = elems[i].first[idx] - '0';
        else    
            key = elems[i].first[idx] - 'a' + 10;

        int pos = tmp[key]--;
        tmpResult[pos-1] = elems[i];
    }

    int i = 0;
    for(auto el : tmpResult) 
        elems[i++] = el;
}


void radix_sort( Vector<Pair>& elems ) {
    
    if(elems.empty())
        return;

    Vector<Pair> tmpResult( elems.get_size(), true );
    for(int i = key_length - 1; i >= 0; --i) {
        counting_sort(elems, tmpResult, i);
    }

}



int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(0); std::cout.tie(0);


    std::string key;
    uint64_t value;

    Vector<Pair> elems(true);
    elems.reserve(1500000);

    while (std::cin >> key >> value)
    {
        elems.push_back( Pair(key, value) );
    }

    radix_sort( elems );

    for (int i = 0 ; i < elems.get_size() ; ++i)
    {
        std::cout << elems[i].first << '\t' << elems[i].second << '\n';
    }

    return 0;
}
