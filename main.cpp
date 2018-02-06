#include <iostream>
#include <iomanip>
#include <vector>
#include <forward_list>
#include "containers/vector.h"
#include "containers/forward_list.h"

using namespace std;

void test_forward_list() {
    Readable::forward_list<int> l{1, 2, 3, 4, 5, 6};
    l.assign(Readable::next(l.begin(), 1), Readable::next(l.begin(), 3));
    for (auto val:l) {
        std::cout << val << ',';
    }
    cout << endl;
}

void test_vector() {
    Readable::vector<int> v{1, 2, 3};
    Readable::vector<int> vector1{4, 5, 6};
    v.insert(v.begin(), vector1.begin(), vector1.end());
    auto it = v.erase(v.begin(), v.begin() + 2);
    for (auto val:v) {
        cout << val << ',';
    }
    cout << endl << *it;

    Readable::vector<std::string> numbers;

    numbers.push_back("abc");
    std::string s = "def";
    numbers.push_back(std::move(s));

    std::cout << "vector holds: ";
    for (auto &&i : numbers) std::cout << i << ' ';
    std::cout << "\nMoved-from string holds " << s << '\n';
}

int main() {
    test_forward_list();
    test_vector();
    return 0;
}