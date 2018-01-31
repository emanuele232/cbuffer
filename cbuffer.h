#ifndef CBUFFER_H
#define CBUFFER_H

#include <ostream> 
#include <iostream>
#include <cassert>
#include <algorithm>
#include <stdexcept>
#include <iterator> 
#include <cstddef> 

template <typename T>
class cbuffer{

public:
    typedef unsigned int size_type; //tipo della variabile size
    typedef T value_type;            //tipo dei valori da inserire

    //costruttori [METODI FONDAMENTALI]
    cbuffer(): _size(0), _buffer(0), _inizio(-1), _fine(-1) {

        #ifndef NDEBUG
        std::cout << "cbuffer()" << std::endl;
        #endif
    }

    explicit cbuffer(size_type size): _size(0), _buffer(0), _inizio(-1), _fine(-1){
        _buffer = new T[size];
        _size = size;

        #ifndef NDEBUG
        std::cout << "cbuffer(size_type)" << std::endl;
        #endif
    }

    cbuffer(size_type size, const T &value): _size(0), _buffer(0), _inizio(-1), _fine(-1){
        _buffer = new T[size];
        _size = size;
        _inizio = 0;
        _fine = 0;

        try{
            for(size_type i = 0; i < size; ++i){
                _buffer[i] = value;
                _fine++;
            }
        }
        catch(...){
            delete[] _buffer;
            _size = 0;
            _buffer = 0;
            throw;
        }
        #ifndef NDEBUG
        std::cout << "cbuffer(size_type, const T)" << std::endl;
        #endif
    }

    //copy constructor
    cbuffer(const cbuffer &other): _size(0), _buffer(0), _inizio(-1), _fine(-1){
        _buffer = new T[other._size];
        _size = other._size;
        _inizio = other._inizio;
        _fine = other._fine;

        try{
            for(size_type i = 0; i < other._size; i++)
                _buffer[i] = other._buffer[i];
        
            
        }
        catch(...){
            delete[] _buffer;
            _size = 0;
            _buffer = 0;
            throw;
        }
        #ifndef NDEBUG
        std::cout << "cbuffer(const cbuffer) - copy constructor" << std::endl;
        #endif

    }

    //operatore di assegnamento [METODO FONDAMENTALE]
    cbuffer &operator=(const cbuffer &other){
        if(this != &other){
            cbuffer tmp(other);
            this ->swap(tmp);
        }
        return *this;
    }

    //distruttore [METODO FONDAMENTALE]
    ~cbuffer(){
        delete[] _buffer;
        _size = 0;
        _buffer = 0;
        _inizio = -1;
        _fine = -1;

    }

    //metodi vari

    //size java style
    size_type get_size() const {
        return _size;
    }

    //size c++ style
    size_type size() const {
        return _size;
    }

    //valore all'indice i java style
    T get_value(size_type index) const {
        assert(index < _size);
        return _buffer[index];
    }

    //set il valore all'indice i java style
    void set_value(size_type index, const T &value) {
        assert (index < _size);
        _buffer[index] = value;
    }

    //valore all'indice i c++ style [LETTURA E SCRITTURA]
    T &value(size_type index) {
		assert(index < _size); // asserzione se viene violata il programma termina
		return _buffer[index];
	}

    //set value all'indice i c++ style [SOLO LETTURA]
    const T &value(size_type index) const {
		assert(index < _size); // asserzione se viene violata il programma termina
		return _buffer[index]; 
	}


    //operatore [] [LETTURA E SCRITTURA]
    T &operator[](size_type index){
        assert(index >= _inizio && index <= _fine);
        return _buffer[index];
    }

    //operatore [] [LETTURA]
    const T &operator[](size_type index) const {
        assert(index <= _inizio && index >=_fine);
        return _buffer[index];
    }

    void swap(cbuffer &other) {
		std::swap(other._size, this->_size);
		std::swap(other._buffer, this->_buffer);
        std::swap(other._inizio, this->_inizio);
        std::swap(other._fine, this->_fine);
	}

    void enqueue(const T &value){
        if (_fine == _inizio && _inizio == -1){ //coda vuota 
            _inizio = (_inizio + 1)%_size;
            _fine = (_fine + 1)%_size;
        }
        else if (_inizio == (_fine + 1)%_size){
            _inizio = (_inizio + 1)%_size;
            _fine = (_fine + 1)%_size;
        }
        else 
            _fine = _fine + 1;

        _buffer[_fine] = value;
    }

    void dequeue(){
        if(_inizio == _fine){
            _inizio = -1;
            _fine = -1;
        }
        else 
            _fine = (_fine + _size - 1)%_size;
 
    }

    class const_iterator; //forward declaration

    class iterator{
        cbuffer *pnt;
        int index;
    
    public:
        typedef std::forward_iterator_tag iterator_category;
        typedef T                         value_type;
        typedef ptrdiff_t                 difference_type;
        typedef T*                        pointer;
        typedef T&                        reference;
    

        iterator() : pnt(0), index(0) {}

        iterator(const iterator &other) : pnt(other.pnt), index(other.index) {}

        ~iterator() {}

        //ridefinisco l'operatore "="
        iterator &operator=(const iterator &other) {
            pnt = other.pnt;
            index = other.index;
        }

        //ritorna il dato a cui si riferisce l'iteratore 
        reference operator*() const {
            return pnt->_buffer[index];
        }

        //ritorna il puntatore al dato a cui si riferisce l'iteratore
        pointer operator->() const{
            reference x = pnt->_buffer[index];
            return &x;
        }

        //operatore di post incremento 
        iterator& operator++(int){
            iterator tmp(*this);
            if(index == pnt->_fine )
                index = pnt->_fine;
            else {
                index = (index + 1)%pnt->_size;
            }
            return tmp;
        } 

        //operatore di pre-incremento
        iterator& operator++() {
            if(index == pnt->_fine)
                index = pnt->_inizio;
            else 
                index = (index + 1)%pnt->_size;
            return *this;
            
        }

        //operatore di uguaglianza
        bool operator==(const iterator &other) const {
            pointer x = &(pnt->_buffer[index]);
            pointer y = &(other.pnt->_buffer[index]);
            return x == y;
        }

        bool operator!=(const iterator &other) const {
            pointer x = &(pnt->_buffer[index]);
            pointer y = &(other.pnt->_buffer[index]);
            return x != y;
        }

        friend class const_iterator;

        bool operator==(const const_iterator &other) const {
            pointer x = &(pnt->_buffer[index]);
            pointer y = &(other.pnt->_buffer[index]);
            return x == y;
        }

        bool operator!=(const const_iterator &other) const {
            pointer x = &(pnt->_buffer[index]);
            pointer y = &(other.pnt->_buffer[index]);
            return x != y;
        }

    private:
        friend class cbuffer;

        iterator(cbuffer *pnt, int index): pnt(pnt), index(index) {}


    };//classe iterator

    iterator begin() {
        return iterator(this, this->_inizio);
    }

    iterator end() {
        return iterator(this, this->_fine);
    }


    class const_iterator{
        cbuffer *pnt;
        int index;
    
    public:
        typedef std::forward_iterator_tag iterator_category;
        typedef T                         value_type;
        typedef ptrdiff_t                 difference_type;
        typedef T*                        pointer;
        typedef T&                        reference;
    

        const_iterator() : pnt(0), index(0) {}

        const_iterator(const iterator &other) : pnt(other.pnt), index(other.index) {}

        ~const_iterator() {}

        const_iterator &operator=(const const_iterator &other) {
            pnt = other.pnt;
            index = other.index;
        }

        const_iterator &operator=(const iterator &other) {
            pnt = other.pnt;
            index = other.index;
        }

        reference operator*() const {
            return pnt->_buffer[index];
        }

        pointer operator->() const{
            reference x = pnt->_buffer[index];
            return &x;
        }

        const_iterator& operator++(int){
            const_iterator tmp(*this);
            if(index == pnt->_fine)
                index = pnt->_inizio;
            else 
                index = (index +1)%pnt->_size;
            return tmp;
        } 

        const_iterator& operator++() {
            if(index == pnt->_fine)
                index = pnt->_inizio;
            else 
                index = (index + 1)%pnt->_size;
            return *this;
        }

        bool operator==(const const_iterator &other) const {
            pointer x = &(pnt->_buffer[index]);
            pointer y = &(other.pnt->_buffer[index]);
            return x == y;
        }

        bool operator!=(const const_iterator &other) const {
            pointer x = &(pnt->_buffer[index]);
            pointer y = &(other.pnt->_buffer[index]);
            return x != y;
        }

        friend class iterator;

        bool operator==(const iterator &other) const {
            pointer x = &(pnt->_buffer[index]);
            pointer y = &(other.pnt->_buffer[index]);
            return x == y;
        }

        bool operator!=(const iterator &other) const {
            pointer x = &(pnt->_buffer[index]);
            pointer y = &(other.pnt->_buffer[index]);
            return x != y;
        }

    private:
        friend class cbuffer;

        const_iterator(cbuffer *pnt, int index): pnt(pnt), index(index) {}


    };//classe const iterator

    

private:
    size_type _size;
    T *_buffer;
    int _inizio;
    int _fine;
};

#endif