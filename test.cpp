#include <iostream>
#include "vector.h"

int main() {
    Vector<int> a{1};
    Vector<int> b{2};
    Vector<int> c{3};

    c = b = std::move(a);
}
