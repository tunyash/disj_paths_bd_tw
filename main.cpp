#include <iostream>
#include "src/path-decomposition.h"
#include "tdlib/src/combinations.hpp"

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
        Tree t = PathDecomposition::create_treedec<treedec::comb::PP_FI<Graph>>(g);
        PathDecomposition(g, t);
    } catch(PathDecomposition::CorectnessException &ex) {
        pass = false;
        std::cout << ex.what();
    }
    if (pass == 1) exit(1);
}
