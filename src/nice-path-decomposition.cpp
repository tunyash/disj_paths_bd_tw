#include "nice-path-decomposition.h"
#include "../tdlib/src/iter.hpp"

std::vector<NicePathDecomposition::Bag> NicePathDecomposition::get_nice_bags() {
    return _nice_bags;
}

void NicePathDecomposition::set_g(Graph &g) {
    _g = g;
}

NicePathDecomposition::NicePathDecomposition(std::vector<std::vector<vertex_t>> &bags, Graph &g) : PathDecomposition(
        bags, g) {
    fill_nice_bags();
}


NicePathDecomposition::NicePathDecomposition(PathDecomposition pathDecomposition) : PathDecomposition(
        pathDecomposition) {
    fill_nice_bags();
}


bool NicePathDecomposition::Bag::operator<(Bag b) {
    if (type == REMOVE_VERTEX && b.type != REMOVE_VERTEX) return true;
    if (type != REMOVE_VERTEX && b.type == REMOVE_VERTEX) return false;
    if (type == REMOVE_VERTEX && b.type == REMOVE_VERTEX) return v < b.v;
    int v1, v2;
    if (type == ADD_VERTEX) v1 = v;
    else v1 = std::max(edge.m_target, edge.m_source);
    if (b.type == ADD_VERTEX) v2 = b.v;
    else v2 = std::max(b.edge.m_target, b.edge.m_source);
    return (v1 < v2) || (v1 == v2 && type == ADD_VERTEX);
}

void NicePathDecomposition::fill_nice_bags() {
    int n = _g.m_vertices.size();
    std::vector<int> left(n, n + 1);
    std::vector<int> right(n, -1);
    for (int i = 0; i < _bags.size(); ++i) {
        for (vertex_t v : _bags[i]) {
            left[v] = std::min(left[v], i);
            right[v] = std::max(right[v], i);
        }
    }
    std::vector<std::vector<Bag>> nice_order(_bags.size() + 1);
    int sz = 0;
    for (vertex_t v = 0; v < n; v++) {
        nice_order[left[v]].emplace_back(bag_types::ADD_VERTEX, v);
        nice_order[right[v] + 1].emplace_back(bag_types::REMOVE_VERTEX, v);
        sz += 2;
    }
    for (auto it = boost::edges(_g).first; it != boost::edges(_g).second; it++) {
        vertex_t v = it->m_source, u = it->m_target;
        int place = std::max(left[v], left[u]);
        nice_order[place].emplace_back(bag_types::ADD_EDGE, *it);
        sz++;
    }
    _nice_bags.resize(sz);
    sz = 0;
    for (int i = 0; i < _bags.size() + 1; i++) {
        sort(nice_order[i].begin(), nice_order[i].end());
        for (Bag b: nice_order[i]) {
            _nice_bags[sz++] = b;
        }
    }

}


bool NicePathDecomposition::is_valid() {
    int n = _g.m_vertices.size();
    std::vector<bool> added(n, false);
    std::vector<bool> removed(n, false);
    std::map<edge_t, bool> edge_used;
    for (Bag bag : _nice_bags) {
        if (bag.type == ADD_VERTEX) added[bag.v] = true;
        else if (bag.type == REMOVE_VERTEX) removed[bag.v] = true;
        else edge_used[bag.edge] = true;
    }
    for (vertex_t v = 0; v < n; v++) {
        if (!added[v]) {
            std::cerr << "vertex " << v << " haven't been added";
            return false;
        }
        if (!removed[v]) {
            std::cerr << "vertex " << v << " haven't been removed";
            return false;
        }
    }

    for (auto it = boost::edges(_g).first; it != boost::edges(_g).second; it++) {
        if (!edge_used.count(*it)) {
            std::cerr << "edge " << it->m_source << ' ' << it->m_target << " haven't been added";
            return false;
        }
    }

    return true;
}