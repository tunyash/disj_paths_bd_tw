#include "nice-path-decomposition.h"
#include "../../tdlib/src/iter.hpp"

std::vector<NicePathDecomposition::Bag> NicePathDecomposition::get_nice_bags() {
    return _nice_bags;
}

void NicePathDecomposition::set_g(Graph &g) {
    _g = g;
}

NicePathDecomposition::NicePathDecomposition(std::vector<std::vector<vertex_t>> &bags, Graph &g) : PathDecomposition(
        bags, g) {
    set_g(g);
    nice_transform();
}


NicePathDecomposition::NicePathDecomposition(PathDecomposition pathDecomposition) : PathDecomposition(
        pathDecomposition) {
    nice_transform();
}

void NicePathDecomposition::nice_transform() {
    std::vector<vertex_t> prev_bag = {};
    std::set<vertex_t> prev_set = {};
    for (const std::vector<vertex_t>& bag : _bags) {
        std::set<vertex_t> vertices;
        for (vertex_t v : bag) vertices.insert(v);
        for (int i = 0; i < prev_bag.size(); i++) {
            vertex_t v = prev_bag[i];
            if (vertices.count(v)) vertices.erase(v);
            else {
                prev_bag.erase(prev_bag.begin() + i);
                prev_set.erase(v);
                i--;
                _nice_bags.emplace_back(bag_types::REMOVE_VERTEX, v);
            }

        }
        for (vertex_t v : vertices) {
            prev_set.insert(v);
            _nice_bags.emplace_back(bag_types::ADD_VERTEX, v);

            for (auto it = boost::out_edges(v, _g).first; it != boost::out_edges(v, _g).second; ++it) {
                if (prev_set.count(it->m_target))
                    _nice_bags.emplace_back(bag_types::ADD_EDGE, *it);
            }
        }
        prev_bag = bag;
    }
}
