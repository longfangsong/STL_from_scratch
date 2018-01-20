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
    Readable::forward_list<int> l{3, 4, 1, 3, 6, 3};
    Readable::forward_list<int> l1{4, 1, 2, 4, 5, 6};
    l.splice_after(l.before_begin(), l1, next(l1.begin(), 4));
    for (auto n:l) {
        cout << n << ',';
    }
    cout << endl;
    for (auto n:l1) {
        cout << n << ',';
    }
    cout << endl;
    return 0;
}