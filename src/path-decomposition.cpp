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
        (int i, int j, int k, int u) :
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

PathDecomposition::PathDecomposition(std::vector<std::vector<vertex_t>> &bags, Graph &g) : _bags(bags), _g(g) {
    check();

}



