#include <iostream>
#include <iomanip>
#include <vector>
#include "containers/vector.h"

using namespace std;

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
    string s;
    test_vector();
    return 0;
}