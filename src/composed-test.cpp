#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
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

TEST_CASE("get_good_subgraph fucntion") {
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
        }
        CHECK(pass);
    }
}