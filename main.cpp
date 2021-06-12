#include <iostream>
#include "src/path-decomposition.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
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

    PathDecomposition q(g);
    q.
    return 0;
}
