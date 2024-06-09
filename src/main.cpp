#include "pq.h"
#include <vector>
#include <string>

int main() {
    pq();

    std::vector<std::string> vec;
    vec.push_back("test_package");

    pq_print_vector(vec);
}
