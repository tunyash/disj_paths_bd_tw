//#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <iostream>
#include <vector>
#include "../doctest/doctest/doctest.h"
#define BOOL BOOLTD
#include "../tdlib/src/combinations.hpp"
#undef BOOL
#include "path-decomposition.h"
#include "nice-path-decomposition.h"
#include "composed-graph.h"
#include "centroid-tree.h"

std::set<std::pair<vertex_t, vertex_t>> get_set_edges(Graph &g) {
    std::set<std::pair<vertex_t, vertex_t>> res;
    for (auto item = boost::edges(g).first; item != boost::edges(g).second; ++item) {
        vertex_t v = boost::source(*item, g), u = boost::target(*item, g);
        res.insert({std::min(v, u), std::max(v, u)});
    }
    return res;
}

TEST_CASE("get_good_subgraph") {
    SUBCASE("Binary tree") {
        int n = 7;
        std::vector<std::pair<int, int>> edges = {
                {0, 1}, {0, 2},
                {1, 3}, {1, 4},
                {2, 5}, {2, 6}
        };
        Graph g(edges.begin(), edges.end(), n);

        int m = 3;
        std::set<std::pair<vertex_t, vertex_t>> need_edges = {
                {0, 1}, {0, 2}
        };

        std::vector<vertex_t> U = {1, 3, 4};
        Graph res = get_good_subgraph(g, U);
        CHECK(get_set_edges(res) == need_edges);
        CHECK(boost::num_vertices(res) == m);
    } SUBCASE("Exception check") {
        Graph g;
        std::vector<vertex_t> U = {27, 31};
        bool pass = false;
        try {
            get_good_subgraph(g, U);
        } catch (std::out_of_range) {
            pass = true;
        }
        CHECK(pass);
    } SUBCASE("No edges in subgraph") {
        int n = 7;
        std::vector<std::pair<int, int>> edges = {
                {0, 1}, {0, 2},
                {1, 3}, {1, 4},
                {2, 5}, {2, 6}
        };
        Graph g(edges.begin(), edges.end(), n);

        int m = 3;
        std::set<std::pair<vertex_t, vertex_t>> need_edges = {
        };

        std::vector<vertex_t> U = {0, 4, 6};
        Graph res = get_good_subgraph(g, U);
        CHECK(get_set_edges(res) == need_edges);
        CHECK(boost::num_vertices(res) == m);
    }
}

TEST_CASE("Constructor ComposedGraph exceptions") {
    SUBCASE("Incomplete union of sets") {
        int n = 6;
        std::vector<std::pair<int, int>> edges = {
                {0, 1}, {1, 2}, {0, 2},
                {3, 4}, {4, 5}, {3, 5},
                {0, 3}, {1, 4}, {2, 5}
        };
        Graph g(edges.begin(), edges.end(), 6);
        std::vector<std::vector<vertex_t>> U = {
                {0, 1}, {3, 4}
        };
        std::vector<NicePathDecomposition> NPD(2);

        bool pass = false;

        try {
            ComposedGraph CG(g, U, NPD);
        } catch(ComposedGraph::CorrectnessException ex) {
            pass = true;
            std::cerr << ex.what() << "\n";
        }
        CHECK(pass);
    } SUBCASE("Non empty intersection exception") {
        int n = 6;
        std::vector<std::pair<int, int>> edges = {
                {0, 1}, {1, 2}, {0, 2},
                {3, 4}, {4, 5}, {3, 5},
                {0, 3}, {1, 4}, {2, 5}
        };
        Graph g(edges.begin(), edges.end(), 6);
        std::vector<std::vector<vertex_t>> U = {
                {0, 1}, {1, 4}
        };
        std::vector<NicePathDecomposition> NPD(2);

        bool pass = false;

        try {
            ComposedGraph CG(g, U, NPD);
        } catch(ComposedGraph::CorrectnessException ex) {
            pass = true;
            std::cerr << ex.what() << "\n";
        }
        CHECK(pass);
    } SUBCASE("Outer edges exception") {
        int n = 6;
        std::vector<std::pair<int, int>> edges = {
                {0, 1}, {1, 2}, {0, 2},
                {3, 4}, {4, 5}, {3, 5},
                {0, 3}, {0, 4}
        };
        Graph g(edges.begin(), edges.end(), 6);
        std::vector<std::vector<vertex_t>> U = {
                {0, 1}, {3, 4}
        };
        std::vector<NicePathDecomposition> NPD(2);

        bool pass = false;

        try {
            ComposedGraph CG(g, U, NPD);
        } catch(ComposedGraph::CorrectnessException ex) {
            pass = true;
            std::cerr << ex.what() << "\n";
        }
        CHECK(pass);
    }
}

TEST_CASE("CompressedGraph") {
    SUBCASE("Simple graph") {
        int n = 4;
        std::vector<std::pair<int, int>> edges = {
                {0, 1}, {2, 3},
                {0, 3}, {1, 2}
        };
        Graph g(edges.begin(), edges.end(), n);

        int m = 2;
        std::set<std::pair<vertex_t, vertex_t>> need_edges = {
                {0, 1}
        };

        std::vector<std::vector<vertex_t>> U = {{0, 1}, {2, 3}};
        std::vector<NicePathDecomposition> NPD(7);
        ComposedGraph CG(g, U, NPD);
        Graph res = CG.get_compressed_graph();
        CHECK(get_set_edges(res) == need_edges);
        CHECK(boost::num_vertices(res) == m);
    } SUBCASE("Matching") {
        int n = 6;
        std::vector<std::pair<int, int>> edges = {
                {0, 5}, {1, 3},
                {2, 4}
        };
        Graph g(edges.begin(), edges.end(), n);

        int m = 6;
        std::set<std::pair<vertex_t, vertex_t>> need_edges = {
                {0, 5}, {1, 3}, {2, 4}
        };

        std::vector<std::vector<vertex_t>> U = {{0}, {1}, {2}, {3}, {4}, {5}};
        std::vector<NicePathDecomposition> NPD(7);
        ComposedGraph CG(g, U, NPD);
        Graph res = CG.get_compressed_graph();
        CHECK(get_set_edges(res) == need_edges);
        CHECK(boost::num_vertices(res) == m);
    } SUBCASE("Empty graph") {
        int n = 0;
        std::vector<std::pair<int, int>> edges = {
        };
        Graph g(edges.begin(), edges.end(), n);

        int m = 0;
        std::set<std::pair<vertex_t, vertex_t>> need_edges = {
        };

        std::vector<std::vector<vertex_t>> U = {};
        std::vector<NicePathDecomposition> NPD(0);
        ComposedGraph CG(g, U, NPD);
        Graph res = CG.get_compressed_graph();
        CHECK(get_set_edges(res) == need_edges);
        CHECK(boost::num_vertices(res) == m);
    }
}

TEST_CASE("ComposedGraph::get_path_decomposition()") {
    SUBCASE("Loop") {
        int n = 6;
        std::vector<std::pair<vertex_t, vertex_t>> edges = {
                {0, 1}, {0, 2}, {2, 3},
                {3, 5}, {5, 4}, {4, 1}
        };
        Graph g(edges.begin(), edges.end(), n);
        std::vector<std::vector<vertex_t>> U = {
                {0, 1}, {2, 3}, {4, 5}
        };
        bool pass = 1;
        try {
            perform_path_dec(g, U);
        } catch(...) {
            pass = 0;
        }
        CHECK(pass);
    } SUBCASE("Star graph") {
        int n = 12;
        std::vector<std::pair<vertex_t, vertex_t>> edges = {
                {0, 11}, {1, 10}, {2, 9}, {3, 8},
                {4, 7}, {5, 6}, {6, 11}, {11, 10},
                {10, 9}, {9, 8}, {8, 7}, {7, 6}
        };
        std::vector<std::vector<vertex_t>> U = {
                {0}, {1}, {2}, {3}, {4}, {5},
                {6, 7, 8, 9, 10, 11}
        };
        Graph g(edges.begin(), edges.end(), n);
        perform_path_dec(g, U);
    }
}