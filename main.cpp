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
    v.enqueue(77);

    v.dequeue();
    v.dequeue();
    v.dequeue();
    
    v.checkout();

    

    return 0;
}

