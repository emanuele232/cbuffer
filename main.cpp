#include "cbuffer.h"
#include <iostream>
#include <cassert>
#include <string>
#include <vector>


//funzioni e costrutti usati nel test di evaluate_if
bool even(int i){
    if ((i % 2) == 0)
        return true;
    else 
    return false;
}

struct testei {
    int primo;
    std::string secondo;
};

bool is_true (testei test){
    if(test.primo == 42 && test.secondo == "foobar")
        return true;
    else 
        return false;
}

int main() {

    
    //test cbuffer();
    std::cout << "\nTEST CREAZIONE CBUFFER VUOTO\n";
    cbuffer<int> d;

    //test cbuffer(size)
    std::cout << "\nTEST CREAZIONE CBUFFER CON SIZE\n";
    cbuffer<int> v(3);

    //test cbuffer(size, &value) [usati operator<< e buffer_size()]
    std::cout << "\nTEST CREAZIONE CBUFFER CON SIZE E RIEMPITO DI VALUE\n";
    cbuffer<std::string> prova(4, "prova");
    std::cout << "\nvalori contenuti nel cbuffer:\n" << prova << std::endl;
    std::cout << "numero di elementi nel cbuffer:\n" << prova.buffer_size() << std::endl;

    //test copy constructor
    //aggiungo due elementi a v
    std::cout << "\nTEST COPY CONSTRUCTOR\n";
    v.push_back(12);
    v.push_back(42);

    cbuffer<int> copy(v);
    std::cout << "elementi in copy:\n" << copy << std::endl;

    //test cbuffer(size, Q iterator_inizio, Q iterator_fine)
    std::cout << "\nTEST CREAZIONE CBUFFER CON SIZE E DUE ITERATORI Q\n";
    std::vector<std::string> a;

    a.push_back("non aggiunto");
    a.push_back("primo");
    a.push_back("secondo");

    std::vector<std::string>::iterator inizio;
    inizio = a.begin();
    inizio++;

    cbuffer <std::string>Qprova(2 , inizio , a.end());

    std::cout << Qprova << std::endl;

    //test Operatore di assegnamento
    std::cout << "\nTEST OPERATORE DI ASSEGNAMENTO\n";
    cbuffer<std::string> assegnamento = prova;

    std::cout << "valori della copia\n" << assegnamento << std::endl;

    //test metodi get/set
    std::cout << "\nTEST METODI GET/SET\n";
    assert(assegnamento.get_value(0) == "prova");
    assegnamento.set_value(1 , "cambiato");
    assert(assegnamento.get_value(1) == "cambiato");

    std::cout << "passed" << std::endl;

    //test metodi value
    std::cout << "\nTEST METODI VALUE\n";
    assert(assegnamento.value(0) == "prova");
    assegnamento.value(1) = "cambiato ancora";
    assert(assegnamento.value(1) == "cambiato ancora");

    std::cout << "passed" << std::endl;

    //test operator []
    std::cout << "\nTEST OPERATORE []\n";
    assert(assegnamento[0] == "prova");
    assegnamento[1] = "cambiato di nuovo";
    assert(assegnamento[1] == "cambiato di nuovo");

    std::cout << "passed" << std::endl;

    //test funzionamento push_back/pop
    std::cout << "\nTEST push back/pop\n";
    cbuffer<int> b(3);

    b.push_back(1);
    b.push_back(2);
    b.push_back(3);

    //cicla dall'elemento più vecchio al più recente
    for(int i = 0; i < b.buffer_size(); i++){
        std:: cout << "elemento " << i << " = " << b[i] << std::endl;
    }

    b.push_back(4);
    b.push_back(5);

    std::cout << "\ndopo le modifiche (pop 4 , 5)\n";

    for(int i = 0; i < b.buffer_size(); i++){
        std:: cout << "elemento " << i << " = " << b[i] << std::endl;
    }

    b.pop();

    std::cout << "\naltre modifiche (pop)\n";
    for(int i = 0; i < b.buffer_size(); i++){
        std:: cout << "elemento " << i << " = " << b[i] << std::endl;
    }

    b.pop();

    std::cout << "\naltre modifiche (pop)\n";
    for(int i = 0; i < b.buffer_size(); i++){
        std:: cout << "elemento " << i << " = " << b[i] << std::endl;
    }

    //test iteratori
    std::cout << "\nTEST ITERATORI\n";
    cbuffer<int>::const_iterator iter_inizio, iter_fine, prova1;  //no accesso in scrittura

    b.push_back(6);
    b.push_back(7);

    iter_inizio = b.begin();
    iter_fine = b.end();

    for(iter_inizio, iter_fine; iter_inizio != iter_fine; iter_inizio++){
        std::cout << "elemento cbuffer: " << *iter_inizio <<std::endl;
    }

    //operatore = negli iteratori
    prova1 = b.begin();
    prova1++;
    assert(b[1] == *prova1);

    std::cout << std::endl;

    //gli altri operatori già testati nel main

    //evaluate_if
    std::cout << "\nTEST EVALUATE_IF" << std::endl;
    std::cout << "cbuffer\n" << b << "funzione: is_even (true if even, else false)" << std::endl;

    std::cout << "\nevaluate_if" << std::endl;
    evaluate_if(b, even);

    std::cout << "\nsecondo test:" << std::endl;
    testei test1 = {
        42,
        "foobar"
    };
    testei test2 = {
        43,
        "foobar"
    };
    testei test3 = {
        42,
        "fooobar"
    };

    cbuffer<testei> testei_cb(3);

    testei_cb.push_back(test1);
    testei_cb.push_back(test2);
    testei_cb.push_back(test3);

    evaluate_if(testei_cb, is_true);
    






    return 0;
}


