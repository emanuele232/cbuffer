#ifndef CBUFFER_H
#define CBUFFER_H

#include <ostream> 
#include <iostream>
#include <cassert>
#include <algorithm>
#include <stdexcept>
#include <iterator> 
#include <cstddef> 

/**
 * @file cbuffer.h
 * @brief Dichiarazione della classe cbuffer
**/

/**
 * @brief Array dinamico di intero 
 * 
 * Classe che rappresenta un buffer circolare che accetta 
 * qualsiasi tipo di dato. La dimensione è fissata a 
 * costruzione
**/
template <typename T>
class cbuffer{

public:
    typedef unsigned int size_type; //tipo della variabile size
    typedef T value_type;            //tipo dei valori da inserire

    /**
     * @brief Costruttore di default (METODO FONDAMENTALE)
     * 
     * costruttore di default per istanziare un cbuffer vuoto.
     * Unico costruttore usabile per creare un array di cbuffer
    **/
    cbuffer(): _size(0), _buffer(0), _inizio(-1), _fine(-1) {

        #ifndef NDEBUG
        std::cout << "\ncbuffer()" << std::endl;
        #endif
    }

    /**
     * @brief Costruttore secondario
     * 
     * Usato per istanziare un cbuffer con una data dimensione .
     * @param size Dimensione del buffer. 
    **/

    explicit cbuffer(size_type size): _size(0), _buffer(0), _inizio(-1), _fine(-1){
        _buffer = new T[size];
        _size = size;

        #ifndef NDEBUG
        std::cout << "\ncbuffer(size_type)" << std::endl;
        #endif
    }

    /**
     * @brief Costruttore secondario.
     * 
     * Usato per istanziare un cbuffer di grandezza size con 
     * valori value
     * 
     * @param size grandezza del buffer.
     * @param value valore con cui inizializzare il buffer
    **/

    cbuffer(size_type size, const T &value): _size(0), _buffer(0), _inizio(-1), _fine(-1){
        _buffer = new T[size];
        _size = size;

        try{
            for(size_type i = 0; i < size; i++){
                push_back(value);
            }
        }
        catch(...){
            delete[] _buffer;
            _size = 0;
            _buffer = 0;
            throw;
        }
        #ifndef NDEBUG
        std::cout << "\ncbuffer(size_type, const T)" << std::endl;
        #endif
    }

    /**
     * @brief Copy constructor.
     * 
     * inizializza un cbuffer a partire da un altro cbuffer.
     * 
     * @param other cbuffer usato per costruire il nuovo cbuffer.
    **/
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
        std::cout << "\ncbuffer(const cbuffer) - copy constructor" << std::endl;
        #endif

    }

    /*
    *@brief costruttore cbuffer data un size e due iteratori generici
    *
    *Questo costruttore istanzia un cbuffer di un certa dimensione con i valori
    *indicati da una coppia di iteratori generici Q.
    *il tipo T del cbuffer deve essere lo stesso degli iteratori di tipo Q.
    *il compilatore gestisce la conversione da Q a T.
    */ 
    template<typename Q> 
    cbuffer(size_type size, Q _begin, Q _end){
        _buffer = new T[size];
        _size = size;
        _inizio = -1;
        _fine = -1;
        int i = 0;

        try{
            for(_begin, _end ; _begin != _end; _begin++, i++){
                push_back(*_begin);
            }
        }
        catch(...){
            delete[] _buffer;
            _size = 0;
            _buffer = 0;
            throw;
        }

        #ifndef NDEBUG
        std::cout << "\ncbuffer (Q,T)" << std::endl;
        #endif
    }

    /**
     * @brief Operatore di assegnamento.
     * 
     * permette la copia tra cbuffer.
     * 
     * @param other cbuffer sorgente.
    **/ 
    cbuffer &operator=(const cbuffer &other){
        if(this != &other){
            cbuffer tmp(other); //creiamo un nuovo cbuffer temporaneo

            //modifichiamo i puntatori del vecchio cbuffer per
            //fargli puntare il nuovo cbuffer. 
            this ->swap(tmp);   
        }
        return *this;
    }

    /** @brief Distruttore della classe cbuffer
     * 
     * svuota la memoria allocata.
    **/
    ~cbuffer(){
        delete[] _buffer;
        _size = 0;
        _buffer = 0;
        _inizio = -1;
        _fine = -1;

    }

    //utilities

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
        assert(index < buffer_size());
        int posizione = (_inizio + index)%_size;
        return _buffer[posizione];
    }

    //set il valore all'indice i java style
    void set_value(size_type index, const T &value) {
        assert(index < buffer_size());
        int posizione = (_inizio + index)%_size;
        _buffer[posizione] = value;
    }

    //valore all'indice i c++ style [LETTURA E SCRITTURA]
    T &value(size_type index) {
        assert(index < buffer_size());
		int posizione = (_inizio + index)%_size;
		return _buffer[posizione];
	}

    //set value all'indice i c++ style [SOLO LETTURA]
    const T &value(size_type index) const {
		assert(index < buffer_size()); // asserzione se viene violata il programma termina
        int posizione = (_inizio + index)%_size;
		return _buffer[posizione]; 
	}

    /**
      @brief operatore [] [LETTURA E SCRITTURA]
      *vogliamo accedere all'elemento di posizione "index"
      *dall'inizio del cbuffer (NON DALL'INIZIO DEL BUFFER)
      *e quindi sommiamo di quanti inizi l'elemento è discosto
      *dall'inizio logico del cbuffer, in quanto l'inizio 
      *dell'allocazione di memoria e l'inizio logico del cbuffer
      *potebbero non coincidere
    **/
    T &operator[](size_type index){
        assert(index < buffer_size());
        int posizione = (_inizio + index)%_size;
        return _buffer[posizione];
    }

    //operatore [] [LETTURA]
    const T &operator[](size_type index) const {
        assert(index < buffer_size());
        int posizione = (_inizio + index)%_size;

        return _buffer[posizione];
    }

    /**
     * @brief buffer_size è la memoria in uso al momento
     * 
     * buffer_size restituisce quante celle sono usate al 
     * momento della chiamata sfruttando gli indici logici
     * _inizio e _fine del buffer
    **/
    int buffer_size() const{
        if(_inizio == -1)
            return 0;
        else if(_inizio == _fine)
            return 1;
        else if(_inizio < _fine)
            return _fine - _inizio + 1;
        else {
            return (_size - _inizio) + _fine + 1;
        } 
    }

    /** @brief metodo di appoggio swap.
     * 
     * Scabio i puntatori di un cbuffer con un altro.
     * 
     * @param other cbuffer sorgente.
    **/
    void swap(cbuffer &other) {
		std::swap(other._size, this->_size);
		std::swap(other._buffer, this->_buffer);
        std::swap(other._inizio, this->_inizio);
        std::swap(other._fine, this->_fine);
	}

    /** @brief inserimento in coda
     * 
     * Enqueue inserisce in coda al buffer il valore value.
     * 
     * @param value valore da inserire in coda.
    **/
    void push_back(const T &value){
        if (_fine == _inizio && _inizio == -1){ //coda vuota 
            _inizio = 0;
            _fine = 0;
        }
        else if (_inizio == (_fine + 1)%_size){
            _inizio = (_inizio + 1)%_size;
            _fine = (_fine + 1)%_size;
        }
        else 
            _fine = (_fine + 1)%_size;

        _buffer[_fine] = value;
    }


    /**@ brief Cancellazione in testa.
     * 
     * Dequeue cancella l'elemento in testa al buffer.
    **/
    void pop(){
        if(_inizio == _fine){
            _inizio = -1;
            _fine = -1;
        }
        else 
            _fine = (_fine + _size - 1)%_size;
    }

    class const_iterator; //forward declaration


    //implementazione della classe iterator per cbuffer.
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

        iterator(const iterator &other) : pnt(other.pnt), index(other.index) {
            #ifndef NDEBUG
            std::cout << "\niterator(iterator &other)" << std::endl;
            #endif
        }

        ~iterator() {}

        //ridefinisco l'operatore "="
        iterator& operator=(const iterator &other) {
            pnt = other.pnt;
            index = other.index;
        }

        //ritorna il dato a cui si riferisce l'iteratore 
        reference operator*() const {
            assert(index < pnt->_size);
            return pnt->_buffer[index];
        }

        //ritorna il puntatore al dato a cui si riferisce l'iteratore
        pointer operator->() const{
            reference x = pnt->_buffer[index];
            return &x;
        }

        //operatore di post incremento 
        iterator operator++(int){
            assert(index < pnt->_size);
            iterator tmp(*this);
            if(index == pnt->_fine )
                index = pnt->_size;
            else {
                index = (index + 1)%pnt->_size;
            }
            return tmp;
        } 

        //operatore di pre-incremento
        iterator& operator++() {
            assert(index < pnt->_size);
            if(index == pnt->_fine)
                index = pnt->_size;
            else 
                index = (index + 1)%pnt->_size;
            return *this;
            
        }

        //operatore di uguaglianza
        bool operator==(const iterator &other) const {
            pointer x = &(pnt->_buffer[index]);
            pointer y = &(other.pnt->_buffer[other.index]);
            return x == y;
        }

        //operatore di disuguaglianza
        bool operator!=(const iterator &other) const {
            pointer x = &(pnt->_buffer[index]);
            pointer y = &(other.pnt->_buffer[other.index]);
            return x != y;
        }

        friend class const_iterator;

        bool operator==(const const_iterator &other) const {
            pointer x = &(pnt->_buffer[index]);
            pointer y = &(other.pnt->_buffer[other.index]);
            return x == y;
        }

        bool operator!=(const const_iterator &other) const {
            pointer x = &(pnt->_buffer[index]);
            pointer y = &(other.pnt->_buffer[other.index]);
            return x != y;
        }

    private:
        friend class cbuffer;

        iterator(cbuffer *pnt, int index): pnt(pnt), index(index) {
            #ifndef NDEBUG
            std::cout << "\niterator(*pnt, index)" << std::endl;
            #endif
        }


    };//classe iterator


    /**
     * @brief iteratore al primo elemento del buffer
     * 
     * begin ritorna un oggetto iteratore posizionato al 
     * primo elemento del buffer in questione.
    **/ 
    iterator begin() {
        return iterator(this , this->_inizio);
    }

    /**@brief Iteratore all'ultimo elemento del buffer
     * 
     * end ritorna un oggetto iteratore posizionato 
     * all'ultimo elemento del buffer.
    **/
    iterator end() {
        return iterator(this, this->_size);
    }


    //implementazione della classe const_iterator
    class const_iterator{
        const cbuffer *pnt;
        int index;
    
    public:
        typedef std::forward_iterator_tag iterator_category;
        typedef T                         value_type;
        typedef ptrdiff_t                 difference_type;
        typedef const T*                        pointer;
        typedef const T&                        reference;
    

        const_iterator() : pnt(0), index(0) {}

        const_iterator(const const_iterator &other) : pnt(other.pnt), index(other.index) {}

        const_iterator(const iterator &other) : pnt(other.pnt), index(other.index) {}

        ~const_iterator() {}

        //operatore di assegnamento (const_iterator sorgente)
        const_iterator& operator=(const const_iterator &other) {
            pnt = other.pnt;
            index = other.index;
            return *this;
        }

        //operatore di assegnamento (iterator sorgente)
        const_iterator& operator=(const iterator &other) {
            pnt = other.pnt;
            index = other.index;
            return *this;
        }

        //operatore di deferenziazione ritorna l'elemento al
        //quale 'literatore si riferisce
        reference operator*() const {
            assert(index < pnt->_size);
            return pnt->_buffer[index];
        }

        //ritorna il puntatore all'elemento al quale l'iteratore
        //si riferisce
        pointer operator->() const{
            reference x = pnt->_buffer[index];
            return &x;
        }

        //Operatore di post incremento
        const_iterator operator++(int){
            const_iterator tmp(*this);
            if(index == pnt->_fine)
                index = pnt->_size;
            else 
                index = (index +1)%pnt->_size;
            return tmp;
        } 

        //Operatore di pre incremento
        const_iterator& operator++() {
            if(index == pnt->_fine)
                index = pnt->_size;
            else 
                index = (index + 1)%pnt->_size;
            return *this;
        }

        //Operatore di uguaglianza
        bool operator==(const const_iterator &other) const {
            pointer x = &(pnt->_buffer[index]);
            pointer y = &(other.pnt->_buffer[other.index]);
            return x == y;
        }

        //Operatore di disuguaglianza
        bool operator!=(const const_iterator &other) const {
            pointer x = &(pnt->_buffer[index]);
            pointer y = &(other.pnt->_buffer[other.index]);
            return x != y;
        }

        friend class iterator;

        bool operator==(const iterator &other) const {
            pointer x = &(pnt->_buffer[index]);
            pointer y = &(other.pnt->_buffer[other.index]);
            return x == y;
        }

        bool operator!=(const iterator &other) const {
            pointer x = &(pnt->_buffer[index]);
            pointer y = &(other.pnt->_buffer[other.index]);
            return x != y;
        }

    private:
        friend class cbuffer;

        const_iterator(const cbuffer *pnt, int index): pnt(pnt), index(index) {}


    };//classe const iterator

    /**
     * @brief iteratore al primo elemento del buffer
     * 
     * begin ritorna un oggetto iteratore costante posizionato al 
     * primo elemento del buffer in questione.
    **/ 
    const_iterator begin() const {
        return const_iterator(this, this->_inizio);   
    }

    /**@brief Iteratore all'ultimo elemento del buffer
     * 
     * end ritorna un oggetto iteratore costante posizionato 
     * all'ultimo elemento del buffer.
    **/
    const_iterator end() const {
        return const_iterator(this, this->_size);
    }

    

private:
    size_type _size;       //dimensione del buffer
    T *_buffer;            //puntatore al buffer
    int _inizio;
    int _fine;
};

template <typename T>
std::ostream& operator<<(std::ostream &os, 
	const cbuffer<T> & cb) {

	for (typename cbuffer<T>::size_type i = 0; i < cb.buffer_size(); ++i)
		os << cb[i] << std::endl;

	return os;
}

template <typename T, typename P>
void inline evaluate_if(const cbuffer<T> cb, const P &predicato){
    for (int i = 0; i < cb.buffer_size(); i++){

        if( predicato(cb[i]) == 1 )  std::cout << "[" << i << "]: true" << std::endl;
        else std::cout << "[" << i << "]: false" << std::endl;
    } 
}

#endif