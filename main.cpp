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
    Readable::forward_list<int> l;
    auto it = l.insert_after(l.before_begin(), 10, 1);
    l.insert_after(it, 2);
    for (auto i:l) {
        cout << i;
    }
    return 0;
}