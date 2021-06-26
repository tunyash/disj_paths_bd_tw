#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <iostream>
#include <vector>
#include <random>
#include "../doctest/doctest/doctest.h"
#include "centroid-tree.h"

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
        c.get_path_decomposition(g);
    } catch(...) {
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
        c.get_path_decomposition(g);
    } catch(...) {
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
        c.get_path_decomposition(g);
    } catch(...) {
        pass = false;
    }
    CHECK(pass);
}

TEST_CASE("empty graph") {
    int n = 4;
    std::vector<std::pair<int, int>> edges = {};

    Graph g(edges.begin(), edges.end(), n);
    bool pass = true;
    try {
        CentroidTree c(g);
        c.get_path_decomposition(g);
    } catch(...) {
        pass = false;
    }
    CHECK(pass);
}

TEST_CASE("not connected graph") {
    int n = 8;
    std::vector<std::pair<int, int>> edges = {
            {0, 1}, {1, 2}, {1, 3},
            {4, 5}, {5, 6}, {6, 7}
    };

    Graph g(edges.begin(), edges.end(), n);
    bool pass = true;
    try {
        CentroidTree c(g);
        c.get_path_decomposition(g);
    } catch(...) {
        pass = false;
    }
    CHECK(pass);
}

TEST_CASE("random graph") {
    srand(2281337);

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
        c.get_path_decomposition(g);
    } catch(...) {
        pass = false;
    }
    CHECK(pass);
}

TEST_CASE("random tree") {
    srand(2281337);

    int n = rand() % 100;
    std::vector<std::pair<int, int>> edges;
    for (int i = 1; i < n; ++i) {
        edges.push_back(std::make_pair(i, rand() % i));
    }

    Graph g(edges.begin(), edges.end(), n);
    bool pass = true;
    try {
        CentroidTree c(g);
        c.get_path_decomposition(g);
    } catch(...) {
        pass = false;
    }
    CHECK(pass);
}

TEST_CASE("sparse grid") {
    srand(2281337);

    int n = 20;
    std::vector<int> X = {1, 0};
    std::vector<int> Y = {0, 1};
    std::vector<std::pair<int, int>> edges;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            for (int go = 0; go < 2; ++go) {
                if (i + X[go] < n && j + Y[go] < n && rand() % 2) {
                    int v = i * n + j, u = (i + X[go]) * n + j + Y[go];
                    edges.push_back(std::make_pair(v, u));
                }
            }
        }
    }

    Graph g(edges.begin(), edges.end(), n * n);
    bool pass = true;
    try {
        CentroidTree c(g);
        c.get_path_decomposition(g);
    } catch(...) {
        pass = false;
    }
    CHECK(pass);
}

TEST_CASE("complete grid") {
    srand(2281337);

    int n = 10;
    std::vector<int> X = {1, 0};
    std::vector<int> Y = {0, 1};
    std::vector<std::pair<int, int>> edges;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            for (int go = 0; go < 2; ++go) {
                if (i + X[go] < n && j + Y[go] < n) {
                    int v = i * n + j, u = (i + X[go]) * n + j + Y[go];
                    edges.push_back(std::make_pair(v, u));
                }
            }
        }
    }

    Graph g(edges.begin(), edges.end(), n * n);
    bool pass = true;
    try {
        CentroidTree c(g);
        c.get_path_decomposition(g);
    } catch(...) {
        pass = false;
    }
    CHECK(pass);
}