#include <iostream>
#include <vector>
#include "src/path-decomposition.h"

int main() {
    int n = 6;
    std::vector<std::pair<int, int>> edges = {
            {0, 1},
            {1, 2},
            {2, 3},
            {3, 4},
            {4, 5},
            {5, 0}
    };

    Graph g(edges.begin(), edges.end(), n);
    bool pass = true;
    try {
        PathDecomposition(g);
    } catch(...) {
        pass = false;
    }
    CHECK(pass);

}
