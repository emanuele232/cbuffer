#include "cbuffer.h"
#include <iostream>
#include <cassert>

int main() {

    cbuffer<int> d;
    cbuffer<int> v(3);
    cbuffer<int> c(4, 4);
    cbuffer<int> e(c);

    v.enqueue(5);
    v.enqueue(6);
    v.enqueue(7);
    v.enqueue(8);
    v.enqueue(9);

    
    
    cbuffer<int>::iterator a = v.begin();

    std::cout << "primo elemento di v: " << *a <<std::endl;
    a++;
    std::cout << "secondo elemento di v: " << *a << std::endl;
    a++;
    std::cout << "terzo elemento di v: " << *a << std::endl;
    a++;
    std::cout << "primo elemento di v: " << *a << std::endl;

    v.dequeue();
    v.dequeue();
    v.dequeue();

    cbuffer<int>iterator a = v.begin();

    std::cout << "primo elemento di v: " << *a << std::endl;
    
    

    return 0;
}

