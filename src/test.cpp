#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <iostream>
#include <vector>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include "path-decomposition.h"

#include "../doctest/doctest/doctest.h"


typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> Graph;

TEST_CASE("loop") {
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
        PathDecomposition q(g);
    } catch(PathDecomposition::CorectnessException &ex) {
        pass = false;
        std::cout << ex.what();
    }
    CHECK(pass);
}