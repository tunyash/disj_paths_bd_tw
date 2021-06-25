#include "../tdlib/src/combinations.hpp"
#include <iostream>
#include <vector>
#include <set>
#include "path-decomposition.h"

PathDecomposition::CorectnessException::CorectnessException(int error_type): _error_type(error_type) {
    switch (_error_type) {
        case EDGES:
            msg = "Not all edges are in path-width decomposition";
        case NEGATIVE_VERTICES:
            msg = "Vertices in bags are not in [0; |V(_g)|)";
        default:
            msg = "Not all vertices are in path-width decomposition";
    }
}
PathDecomposition::CorectnessException::CorectnessException
        (int i, int j, int k, int u):
        _error_type(INCONTINUITY), _bag_i(i), _bag_j(j), _bag_k(k), _violating_node(u) {
    msg = "_bags["
          + std::to_string(_bag_i)
          + "] and _bags["
          + std::to_string(_bag_j)
          + "] contains "
          + std::to_string(_bag_k)
          + " but _bags["
          + std::to_string(_violating_node)
          + "] does not";
}

const char *PathDecomposition::CorectnessException::what() const throw() {
    return msg.c_str();
}

void PathDecomposition::check() {
    int n = boost::num_vertices(_g);

    for (std::vector<vertex_t> &bag : _bags) {
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

    for (int v = 0; v < n; ++v) {
        for (auto it = boost::adjacent_vertices(v, _g).first; it != boost::adjacent_vertices(v, _g).second; ++it) {
            if (left[v] > right[*it] || left[*it] > right[v]) throw PathDecomposition::CorectnessException(EDGES);
        }
    }
}

PathDecomposition::PathDecomposition(std::vector<std::vector<vertex_t>> &bags, Graph &g): _bags(bags), _g(g) {
    check();
}

PathDecomposition::PathDecomposition(Graph &g): _g(g) {
    transform();
    check();
}

void PathDecomposition::transform() {
    Tree t;

    treedec::exact_decomposition_cutset(_g, t);

    auto tree_bags = get(treedec::bag_t(), t);
    int log_n = 0, n = boost::num_vertices(t);
    while ((1 << log_n) < n) ++log_n;
    std::vector<std::vector<int>> size(n, std::vector<int>(log_n + 2));
    // size[v][d] = size of subtree of vertex |v| on depth |d|
    std::vector<int> used(n);
    // used[v] = 1 if vertex |v| is already in centroid tree
    // used[v] = 0 else
    std::vector<int> parent(n, -1);
    // parent[v] = parent of vertex |v| in centroid tree

    std::function<void(int, int, int)> size_calculation;
    size_calculation = [&](tree_vertex v, int depth, tree_vertex p) {
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
    find_centroid = [&](tree_vertex v, int depth, int all, tree_vertex p) {
        for (auto item = boost::adjacent_vertices(v, t).first;
        item != boost::adjacent_vertices(v, t).second;
        ++item) {
            vertex_t u = *item;
            if (p == u || used[u]) continue;
            if (size[u][depth] > all / 2) return find_centroid(u, depth, all, v);
        }
        return v;
    };

    std::function<void(int, int, int)> build_cetroid_tree;
    build_cetroid_tree = [&](tree_vertex v, int depth, tree_vertex p) {
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
            for (tree_vertex u = c; u < n; u = parent[u]) {
                for (vertex_t item : get(tree_bags, u)) {
                    _bags.back().push_back(item);
                }
            }
        }
    };

    build_cetroid_tree(0, 0, -1);
}