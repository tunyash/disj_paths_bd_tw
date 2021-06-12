#include <iostream>
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

    PathDecomposition q(g);
    for (auto bag: q.get_bags()) {
        for (auto node : bag) {
            std::cout << node << std::endl;
        }
    }
    return 0;
}
