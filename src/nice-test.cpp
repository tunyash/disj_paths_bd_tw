#include <iostream>
#include <vector>
#include <random>
#include "../doctest/doctest/doctest.h"
#include "nice-path-decomposition.h"

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
        CentroidTree c(g);
        NicePathDecomposition p(c.get_path_decomposition(g));
    } catch (...) {
        pass = false;
    }

    CHECK(pass);
}

TEST_CASE("Constructor check") {
    int n = 2;
    std::vector<std::pair<int, int>> edges = {
            {0, 1},
    };
    Graph g(edges.begin(), edges.end(), n);
    std::vector<NicePathDecomposition::Bag> nice_bags = {
            {NicePathDecomposition::ADD_VERTEX,    0},
            {NicePathDecomposition::ADD_VERTEX,    1},
            {*boost::edges(g).first},
            {NicePathDecomposition::REMOVE_VERTEX, 0},
            {NicePathDecomposition::REMOVE_VERTEX, 1}
    };
    bool pass = true;
    try {
        NicePathDecomposition p(nice_bags,g);
    } catch (...) {
        pass = false;
    }
    CHECK(pass);
}

TEST_CASE("binary tree") {
    int n = 7;
    std::vector<std::pair<int, int>> edges = {
            {0, 1},
            {0, 2},
            {1, 3},
            {1, 4},
            {2, 5},
            {2, 6}
    };

    Graph g(edges.begin(), edges.end(), n);
    bool pass = true;
    try {
        CentroidTree c(g);
        NicePathDecomposition p(c.get_path_decomposition(g));
    } catch (...) {
        pass = false;
    }
    CHECK(pass);
}

TEST_CASE("complete graph") {
    int n = 4;
    std::vector<std::pair<int, int>> edges = {
            {0, 1},
            {0, 2},
            {0, 3},
            {1, 2},
            {1, 3},
            {2, 3},
    };

    Graph g(edges.begin(), edges.end(), n);
    bool pass = true;
    try {
        CentroidTree c(g);
        NicePathDecomposition p(c.get_path_decomposition(g));
    } catch (...) {
        pass = false;
    }
    CHECK(pass);
}

TEST_CASE("random graph") {
    srand(1000 - 7);

    int n = 15;
    std::vector<std::pair<int, int>> edges;
    edges.resize(rand() % 30);

    for (int i = 0; i < edges.size(); ++i) {
        int v = 0;
        int u = 0;
        while (v == u) {
            v = rand() % n;
            u = rand() % n;
        }
        edges[i] = std::make_pair(v, u);
    }

    Graph g(edges.begin(), edges.end(), n);
    bool pass = true;
    try {
        CentroidTree c(g);
        NicePathDecomposition p(c.get_path_decomposition(g));
    } catch (...) {
        pass = false;
    }
    CHECK(pass);
}

TEST_CASE("random tree") {
    srand(1000 - 7);

    int n = rand() % 100;
    std::vector<std::pair<int, int>> edges;
    for (int i = 1; i < n; ++i) {
        edges.push_back(std::make_pair(i, rand() % i));
    }

    Graph g(edges.begin(), edges.end(), n);
    bool pass = true;
    try {
        CentroidTree c(g);
        NicePathDecomposition p(c.get_path_decomposition(g));
    } catch (...) {
        pass = false;
    }
    CHECK(pass);
}