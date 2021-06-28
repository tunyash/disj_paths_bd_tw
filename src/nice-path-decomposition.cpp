#include "nice-path-decomposition.h"
#include "../tdlib/src/iter.hpp"

const std::vector<NicePathDecomposition::Bag>& NicePathDecomposition::get_nice_bags() {
    return _nice_bags;
}

void NicePathDecomposition::set_g(Graph &g) {
    _g = g;
}

NicePathDecomposition::NicePathDecomposition(std::vector<std::vector<vertex_t>> &bags, Graph &g) : PathDecomposition(
        bags, g) {
    fill_nice_bags();
    is_correct();
}


NicePathDecomposition::NicePathDecomposition(PathDecomposition pathDecomposition) : PathDecomposition(
        pathDecomposition) {
    fill_nice_bags();
    is_correct();
}

NicePathDecomposition::NiceBagsCorrectnessException::NiceBagsCorrectnessException(error_types type, vertex_t v) {
    switch (type) {
        case MISS_VERTEX:
            msg = "vertex " + std::to_string(v) + " haven't been added";
            break;
        case LEFT_VERTEX:
            msg = "vertex " + std::to_string(v) + " haven't been deleted";
            break;
    }
}

NicePathDecomposition::NiceBagsCorrectnessException::NiceBagsCorrectnessException(edge_t edge) {
    msg = "edge " + std::to_string(edge.m_source) + ' ' + std::to_string(edge.m_target) + " haven't been added";
}

const char *NicePathDecomposition::NiceBagsCorrectnessException::what() const throw() {
    return msg.c_str();
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
    std::vector<int> leftmost_bag(n, n + 1);
    std::vector<int> rightmost_bag(n, -1);
    for (int i = 0; i < _bags.size(); ++i) {
        for (vertex_t v : _bags[i]) {
            leftmost_bag[v] = std::min(leftmost_bag[v], i);
            rightmost_bag[v] = std::max(rightmost_bag[v], i);
        }
    }
    std::vector<std::vector<Bag>> nice_order(_bags.size() + 1);
    int sz = 0;
    for (vertex_t v = 0; v < n; v++) {
        nice_order[leftmost_bag[v]].emplace_back(bag_types::ADD_VERTEX, v);
        nice_order[rightmost_bag[v] + 1].emplace_back(bag_types::REMOVE_VERTEX, v);
        sz += 2;
    }
    for (auto it = boost::edges(_g).first; it != boost::edges(_g).second; it++) {
        vertex_t v = it->m_source, u = it->m_target;
        int place = std::max(leftmost_bag[v], leftmost_bag[u]);
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


void NicePathDecomposition::is_correct() {
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
            throw NiceBagsCorrectnessException(MISS_VERTEX, v);
        }
        if (!removed[v]) {
            throw NiceBagsCorrectnessException(LEFT_VERTEX, v);
        }
    }

    for (auto it = boost::edges(_g).first; it != boost::edges(_g).second; it++) {
        if (!edge_used.count(*it)) {
            throw NiceBagsCorrectnessException(*it);
        }
    }

}