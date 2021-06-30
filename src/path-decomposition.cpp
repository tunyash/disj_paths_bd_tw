#include "../tdlib/src/combinations.hpp"
#include <vector>
#include <set>
#include "path-decomposition.h"


PathDecomposition::CorectnessException::CorectnessException(int error_type) : _error_type(error_type) {
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
        (int bag_i, int bag_j, int bag_k, int violating_node) :
        _error_type(INCONTINUITY),
        _bag_i(bag_i),
        _bag_j(bag_j),
        _bag_k(bag_k),
        _violating_node(violating_node) {

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

    std::vector<int> containg_bags_count(n);
    std::vector<int> leftmost_bag(n, n + 1);
    std::vector<int> rightmost_bag(n, -1);

    for (int i = 0; i < _bags.size(); ++i) {
        for (vertex_t v : _bags[i]) {
            containg_bags_count[v]++;
            leftmost_bag[v] = std::min(leftmost_bag[v], i);
            rightmost_bag[v] = std::max(rightmost_bag[v], i);
        }
    }

    for (int i = 0; i < n; ++i) {
        if (containg_bags_count[i] == 0) throw PathDecomposition::CorectnessException(VERTICES);
        if (rightmost_bag[i] - leftmost_bag[i] + 1 != containg_bags_count[i]) {
            int j = leftmost_bag[i];
            for (;
                    std::find(_bags[j].begin(), _bags[j].end(), i) != _bags[j].end();
                    ++j);
            throw PathDecomposition::CorectnessException(leftmost_bag[i], j, rightmost_bag[i], i);
        }
    }

    for (int v = 0; v < n; ++v) {
        for (auto it = boost::adjacent_vertices(v, _g).first; it != boost::adjacent_vertices(v, _g).second; ++it) {
            if (leftmost_bag[v] > rightmost_bag[*it] || leftmost_bag[*it] > rightmost_bag[v])
                throw PathDecomposition::CorectnessException(EDGES);
        }
    }
}

PathDecomposition::PathDecomposition(std::vector<std::vector<vertex_t>> &bags, Graph &g) : _bags(bags), _g(g) {
    check();
}

PathDecomposition::PathDecomposition() {};



