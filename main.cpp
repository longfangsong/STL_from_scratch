#include <iostream>
#include <forward_list>
#include "containers/forward_list.h"

using namespace std;
int cnt = 0;

struct C {
    int n;

    C() : n(++cnt) {
        cout << "Id=" << n << " created!" << endl;
    }

    ~C() {
        cout << "Id=" << n << " deleted!" << endl;
    }
};

int main() {
    Readable::forward_list<int> l{2, 3, 3, 6, 6, 4, 1, 4, 2, 2, 7, 4, 5, 9, 1, 5, 1, 5, 7, 1, 5};
    Readable::forward_list<int> l1(10);
    l.sort();
    for (auto n:l) {
        cout << n << ',';
    }
    cout << endl;

    return 0;
}