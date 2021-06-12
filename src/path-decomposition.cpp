#include "../tdlib/src/combinations.hpp"
#include <iostream>
#include <vector>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/property_map/property_map.hpp>
#include <set>
#include "path-decomposition.h"

PathDecomposition::CorectnessException::CorectnessException(int error_type): _error_type(error_type) {}
PathDecomposition::CorectnessException::CorectnessException
        (int i, int j, int k, int u):
        _error_type(INCONTINUITY), _bag_i(i), _bag_j(j), _bag_k(k), _violating_node(u) {}

const char *PathDecomposition::CorectnessException::what() const throw() {
    switch (_error_type) {
        case EDGES:
            return "Not all edges are in path-width decomposition";
        case NEGATIVE_VERTICES:
            return "Vertices in bags are not in [0; |V(_g)|)";
        case INCONTINUITY:
            return ("_bags["
                    + std::to_string(_bag_i)
                    + "] and _bags["
                    + std::to_string(_bag_j)
                    + "] contains "
                    + std::to_string(_bag_k)
                    + " but _bags["
                    + std::to_string(_violating_node)
                    + "] does not").c_str();
        case VERTICES:
            return "Not all vertices are in path-width decomposition";
    }
}

void PathDecomposition::Check() {
    int n = boost::num_vertices(_g);

    for (std::vector<vertex_t> bag : _bags) {
        std::sort(bag.begin(), bag.end());
        bag.resize(std::unique(bag.begin(), bag.end()) - bag.begin());
        //Erasing non-unique vertices from bags
        for (int v : bag) {
            if (v < 0 || v >= n)
                throw PathDecomposition::CorectnessException(NEGATIVE_VERTICES);
        }
    }

    std::vector<int> sum(n);
    std::vector<int> left(n, n + 1);
    std::vector<int> right(n, -1);

    for (int i = 0; i < _bags.size(); ++i) {
        for (vertex_t v : _bags[i]) {
            sum[v]++;
            left[v] = std::min(left[v], i);
            right[v] = std::max(right[v], i);
        }
    }

    for (int i = 0; i < n; ++i) {
        if (sum[i] == 0) throw PathDecomposition::CorectnessException(VERTICES);
        if (right[i] - left[i] + 1 != sum[i]) {
            int j = left[i];
            for (;
                    std::find(_bags[j].begin(), _bags[j].end(), i) != _bags[j].end();
                    ++j);
            throw PathDecomposition::CorectnessException(left[i], j, right[i], i);
        }
    }

    Graph path_g(n);

    for (std::vector<vertex_t> bag : _bags) {
        for (vertex_t v : bag) {
            for (vertex_t u : bag) {
                if (u == v) continue;
                boost::add_edge(u, v, path_g);
            }
        }
    }

    for (int v = 0; v < n; ++v) {
        for (auto it = boost::adjacent_vertices(v, _g).first; it != boost::adjacent_vertices(v, _g).second; ++it) {
            if (std::find(boost::adjacent_vertices(v, path_g).first, boost::adjacent_vertices(v, path_g).second, *it) ==
                boost::adjacent_vertices(v, path_g).second) throw PathDecomposition::CorectnessException(EDGES);
        }
    }
}

PathDecomposition::PathDecomposition(std::vector<std::vector<vertex_t>> bags, Graph g): _bags(bags), _g(g) {
    Check();
}

PathDecomposition::PathDecomposition(Graph g): _g(g) {
    transform();
    Check();
}

void PathDecomposition::transform() {


    treedec::comb::PP_FI<Graph> algo(_g);
    algo.do_it();
    treedec::graph_traits<Graph>::treedec_type t;
    algo.get_tree_decomposition(t);
    auto tree_bags = get(treedec::bag_t(), t);

    int log_n = 0, n = boost::num_vertices(t);
    while ((1 << log_n) < n) ++log_n;
    std::vector<std::vector<int>> size(n, std::vector<int>(log_n));
    // size[v][d] = size of subtree of vertex |v| on depth |d|
    std::vector<int> used(n);
    // used[v] = 1 if vertex |v| is already in centroid tree
    // used[v] = 0 else
    std::vector<int> parent(n, -1);
    // parent[v] = parent of vertex |v| in centroid tree

    std::function<void(int, int, int)> size_calculation;
    size_calculation = [&](tree_vertex v, int depth, tree_vertex p = -1) {
        size[v][depth] = 1;
        for (auto item = boost::adjacent_vertices(v, t).first;
        item != boost::adjacent_vertices(v, t).second;
        ++item) {
            tree_vertex u = *item;
            if (p == u || used[u]) continue;
            size_calculation(u, depth, v);
            size[v][depth] += size[u][depth];
        }
    };

    std::function<tree_vertex(int, int, int, int)> find_centroid;
    find_centroid = [&](tree_vertex v, int depth, int n = 0, tree_vertex p = -1) {
        for (auto item = boost::adjacent_vertices(v, t).first;
        item != boost::adjacent_vertices(v, t).second;
        ++item) {
            vertex_t u = *item;
            if (p == u || used[u]) continue;
            if (size[u][depth] >= n / 2) return find_centroid(u, depth, n, v);
        }
        return v;
    };

    std::function<void(int, int, int)> build_cetroid_tree;
    build_cetroid_tree = [&](tree_vertex v, int depth = 0, int p = -1) {
        size_calculation(v, depth, -1);
        tree_vertex c = find_centroid(v, depth, size[v][depth], -1);
        used[c] = 1;
        parent[c] = p;
        int adjacency = 0;

        for (auto item = boost::adjacent_vertices(c, t).first;
        item != boost::adjacent_vertices(c, t).second;
        ++item) {
            tree_vertex u = *item;
            if (used[u]) continue;
            adjacency++;
            build_cetroid_tree(u, depth + 1, c);
        }
        if (adjacency == 0) {
            _bags.push_back({});
            for (tree_vertex u = c; u != -1; u = parent[u]) {
                for (vertex_t item : get(tree_bags, u)) {
                    _bags.back().push_back(item);
                }
            }
        }
    };

    build_cetroid_tree(0, 0, -1);
}