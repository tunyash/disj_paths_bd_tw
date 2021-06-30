#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <iostream>
#include <vector>
#include <random>
#include "../doctest/doctest/doctest.h"
#define BOOL BOOLTD
#include "../tdlib/src/combinations.hpp"
#undef BOOL
#include "centroid-tree.h"
#include <set>

TEST_CASE("tests for centroid-tree.h and tdlib.h") {
    SUBCASE("loop") {
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

    SUBCASE("binary tree") {
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

    SUBCASE("complete graph") {
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

    SUBCASE("empty graph") {
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

    SUBCASE("not connected graph") {
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

    SUBCASE("random graph") {
        srand(2281337);

        int n = 10;
        std::set<std::pair<int, int>> edges;

        while (edges.size() < 15) {
            int v = 0;
            int u = 0;
            while (v == u) {
                v = rand() % n;
                u = rand() % n;
            }
            if (v > u) std::swap(v, u);
            edges.insert(std::make_pair(v, u));
        }

        std::vector<std::pair<int, int>> _edges;

        for (auto i : edges) _edges.push_back(i);

        Graph g(_edges.begin(), _edges.end(), n);
        bool pass = true;
        try {
            CentroidTree c(g);
            c.get_path_decomposition(g);
        } catch(...) {
            pass = false;
        }
        CHECK(pass);
    }

    SUBCASE("random tree") {
        srand(2281337);

        int n = 100;
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

//    SUBCASE("sparse grid") {
//        srand(2281337);
//
//        int n = 10;
//        std::vector<int> X = {1, 0};
//        std::vector<int> Y = {0, 1};
//        std::vector<std::pair<int, int>> edges;
//        for (int i = 0; i < n; ++i) {
//            for (int j = 0; j < n; ++j) {
//                for (int go = 0; go < 2; ++go) {
//                    if (i + X[go] < n && j + Y[go] < n && rand() % 2) {
//                        int v = i * n + j, u = (i + X[go]) * n + j + Y[go];
//                        edges.push_back(std::make_pair(v, u));
//                    }
//                }
//            }
//        }
//
//        Graph g(edges.begin(), edges.end(), n * n);
//        bool pass = true;
//        try {
//            CentroidTree c(g);
//            c.get_path_decomposition(g);
//        } catch(...) {
//            pass = false;
//        }
//        CHECK(pass);
//    }
//
//    SUBCASE("complete grid") {
//        srand(2281337);
//
//        int n = 4;
//        std::vector<int> X = {1, 0};
//        std::vector<int> Y = {0, 1};
//        std::vector<std::pair<int, int>> edges;
//        for (int i = 0; i < n; ++i) {
//            for (int j = 0; j < n; ++j) {
//                for (int go = 0; go < 2; ++go) {
//                    if (i + X[go] < n && j + Y[go] < n) {
//                        int v = i * n + j, u = (i + X[go]) * n + j + Y[go];
//                        edges.push_back(std::make_pair(v, u));
//                    }
//                }
//            }
//        }
//
//        Graph g(edges.begin(), edges.end(), n * n);
//        bool pass = true;
//        try {
//            CentroidTree c(g);
//            c.get_path_decomposition(g);
//        } catch(...) {
//            pass = false;
//        }
//        CHECK(pass);
//    }
}


TEST_CASE("custom tree decomposition for centroid-tree.h") {
//for shorten function put()
    SUBCASE("binary tree") {
        int n = 7;
        std::vector<std::pair<int, int>> edges = {
                {0, 1}, {0, 2},
                {1, 3}, {1, 4},
                {2, 5}, {2, 6}
        };
        Graph g(edges.begin(), edges.end(), n);
        std::vector<std::pair<int, int>> tree_edges = {
                {0, 1}, {1, 2}, {0, 3},
                {3, 4}, {3, 5}
        };
        Tree t(tree_edges.begin(), tree_edges.end(), n - 1);

        t[0].bag = {0, 1};
        t[1].bag = {1, 3};
        t[2].bag = {1, 4};
        t[3].bag = {0, 2};
        t[4].bag = {2, 5};
        t[5].bag = {2, 6};

        bool pass = true;
        try {
            CentroidTree c(t);
            c.get_path_decomposition(g);
        } catch(...) {
            pass = false;
        }
        CHECK(pass);
    }

    SUBCASE("loop in tree decomposition") {
        int n = 4;
        std::vector<std::pair<int, int>> tree_edges = {
                {0, 1}, {1, 2}, {2, 3}, {3, 0}
        };
        Tree t(tree_edges.begin(), tree_edges.end(), n);
        bool pass = false;

        try{
            CentroidTree c(t);
        } catch(CentroidTree::TreeException) {
            pass = true;
        }
        CHECK(pass);
    }

    SUBCASE("Loop") {
        std::vector<std::pair<int, int>> edges = {
                {0, 1}, {1, 2}, {2, 3},
                {3, 4}, {4, 5}, {5, 0}
        };

        std::vector<std::pair<int, int>> tree_edges = {
                {0, 1}, {1, 2}, {2, 3}
        };

        Graph g(edges.begin(), edges.end(), 6);
        Tree t(tree_edges.begin(), tree_edges.end(), 4);

        t[0].bag = {0, 1, 2};
        t[1].bag = {0, 2, 3};
        t[2].bag = {0, 3, 4};
        t[3].bag = {0, 4, 5};
        bool pass = true;

        try{
            CentroidTree c(t);
            c.get_path_decomposition(g);
        } catch(...) {
            pass = false;
        }

        CHECK(pass);
    }

    SUBCASE("Complete graph") {
        int n = 6;
        std::vector<std::pair<int, int>> edges;
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                edges.push_back(std::make_pair(i, j));
            }
        }
        Graph g(edges.begin(), edges.end(), n);
        Tree t(1);

        t[0].bag = {0, 1, 2, 3, 4, 5};

        bool pass = true;
        try{
            CentroidTree c(t);
            c.get_path_decomposition(g);
        } catch(...) {
            pass = false;
        }

        CHECK(pass);
    }
}