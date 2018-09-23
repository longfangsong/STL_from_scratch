#include <iostream>
#include "containers/vector.h"
#include "containers/forward_list.h"
#include "containers/list.h"
//#include "containers/deque.h"
using namespace Readable;

void test_forward_list() {
    Readable::forward_list<int> l{1, 2, 3, 4, 5, 6};
    Readable::forward_list<int> l2{1, 2, 3, 4, 5, 6};
    l.assign(Readable::next(l.begin(), 1), Readable::next(l.begin(), 3));
    l.merge(l2);
    for (auto val:l) {
        std::cout << val << ',';
    }
    std::cout << std::endl;
}

void test_vector() {
    Readable::vector<int> v{1, 2, 3};
    Readable::vector<int> vector1{4, 5, 6};
    v.insert(v.begin(), vector1.begin(), vector1.end());
    auto it = v.erase(v.begin(), v.begin() + 2);
    for (auto val:v) {
        std::cout << val << ',';
    }
    std::cout << std::endl << *it;

    Readable::vector<std::string> numbers;

    numbers.push_back("abc");
    std::string s = "def";
    numbers.push_back(std::move(s));

    std::cout << "\nvector holds: ";
    for (auto &&i : numbers) std::cout << i << ' ';
    std::cout << "\nMoved-from string holds " << s << '\n';
}

void test_list() {
    Readable::list<int> l1{1, 3, 5};
    Readable::list<int> l2{2, 4, 6};
    assert(*(l1.begin()) == 1);
    l1.splice(l1.begin(), l2, next(l2.begin()));
    for (auto n: l1) {
        std::cout << n << ',';
    }
    std::cout << std::endl;
    for (auto r = l1.rbegin(); r != l1.rend(); ++r) {
        std::cout << *r << ',';
    }
    std::cout << std::endl;
    for (auto n: l2) {
        std::cout << n << ',';
    }
    std::cout << std::endl;
    for (auto r = l2.rbegin(); r != l2.rend(); ++r) {
        std::cout << *r << ',';
    }
    std::cout << std::endl;
    l1.splice(next(l1.begin()), l2, l2.begin(), l2.end());
    for (auto n: l1) {
        std::cout << n << ',';
    }
    std::cout << std::endl;
    for (auto r = l1.rbegin(); r != l1.rend(); ++r) {
        std::cout << *r << ',';
    }
    std::cout << std::endl;
    l1.push_back(7);
    for (auto n: l1) {
        std::cout << n << ',';
    }
    std::cout << std::endl;
    for (auto r = l1.rbegin(); r != l1.rend(); ++r) {
        std::cout << *r << ',';
    }
    std::cout << std::endl;
    l1.sort();
    for (auto n: l1) {
        std::cout << n << ',';
    }
    std::cout << std::endl;
    for (auto r = l1.rbegin(); r != l1.rend(); ++r) {
        std::cout << *r << ',';
    }
    std::cout << std::endl;
    l1.remove_if([](int a) { return a < 4; });
    for (auto n: l1) {
        std::cout << n << ',';
    }
    std::cout << std::endl;
    for (auto r = l1.rbegin(); r != l1.rend(); ++r) {
        std::cout << *r << ',';
    }
    std::cout << std::endl;
    l1.push_back(3);
    l1.push_back(4);
    l1.push_back(5);
    l1.push_back(6);
    l1.push_back(6);
    l1.sort();
    l1.unique();
    for (auto n: l1) {
        std::cout << n << ',';
    }
    std::cout << std::endl;
    for (auto r = l1.rbegin(); r != l1.rend(); ++r) {
        std::cout << *r << ',';
    }
    std::cout << std::endl;
}

int main() {
//    test_forward_list();
//    test_vector();
    test_list();
    return 0;
}