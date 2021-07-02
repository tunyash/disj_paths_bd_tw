#include "nice-path-decomposition.h"
#include "../tdlib/src/iter.hpp"

const std::vector<NicePathDecomposition::Bag> &NicePathDecomposition::get_nice_bags() {
    return _nice_bags;
}

void NicePathDecomposition::set_g(Graph &g) {
    _g = g;
}


NicePathDecomposition::NicePathDecomposition(std::vector<Bag> &nice_bags, Graph &g) {
    _g = g;
    _nice_bags = nice_bags;
    _bags = convert_nice_bags(nice_bags);
    is_correct();
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

NicePathDecomposition::NiceBagsCorrectnessException::NiceBagsCorrectnessException(error_types type, vertex_t v) : type(
        type) {
    switch (type) {
        case ADD_VERTEX_E:
            msg = "vertex " + std::to_string(v) + " haven't been added or added more than one time";
            break;
        case REMOVE_VERTEX_E:
            msg = "vertex " + std::to_string(v) + " haven't been deleted or deleted more than one time";
            break;
    }
}

NicePathDecomposition::NiceBagsCorrectnessException::NiceBagsCorrectnessException(error_types type, edge_t edge) : type(
        type) {
    switch (type) {
        case (MISS_EDGE):
            msg = "edge " + std::to_string(edge.m_source) + ' ' + std::to_string(edge.m_target) + " haven't been added";
        case (ADD_EDGE_BEFORE_VERTEX):
            msg = "edge " + std::to_string(edge.m_source) + ' ' + std::to_string(edge.m_target) +
                  +" was added in bag without all it's vertices";
    }
}

NicePathDecomposition::NiceBagsCorrectnessException::NiceBagsCorrectnessException(error_types type) : type(type) {
    if (type == EXTRA_EDGE) {
        msg = "nice_bags have some extra edges";
    }
}

const char *NicePathDecomposition::NiceBagsCorrectnessException::what() const throw() {
    return msg.c_str();
}


bool NicePathDecomposition::Bag::operator<(Bag b) {
    if (type == REMOVE_VERTEX && b.type != REMOVE_VERTEX) return true;
    if (type != REMOVE_VERTEX && b.type == REMOVE_VERTEX) return false;
    if (type == REMOVE_VERTEX && b.type == REMOVE_VERTEX) return vertex < b.vertex;
    int v1, v2;
    if (type == ADD_VERTEX) v1 = vertex;
    else v1 = std::max(edge.m_target, edge.m_source);
    if (b.type == ADD_VERTEX) v2 = b.vertex;
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
        nice_order[place].emplace_back(*it);
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
    std::vector<int> added(n, false);
    std::vector<int> removed(n, false);
    std::map<std::pair<int, int>, bool> edge_used;
    int e = 0;
    for (Bag bag : _nice_bags) {
        if (bag.type == ADD_VERTEX) added[bag.vertex]++;
        else if (bag.type == REMOVE_VERTEX) {
            if (!added[bag.vertex])
                throw NiceBagsCorrectnessException(REMOVE_BEFORE_ADDING, bag.vertex);
            removed[bag.vertex]++;
        } else {
            e++;
            vertex_t v = bag.edge.m_source, u = bag.edge.m_target;
            if (v > u) std::swap(v, u);
            std::pair<int, int> edge = std::make_pair(v, u);
            if(edge_used[edge]) throw NiceBagsCorrectnessException(EXTRA_EDGE);
            edge_used[edge] = true;
            if (!added[edge.first] || !added[edge.second] || removed[edge.first] || removed[edge.second]) {
                throw NiceBagsCorrectnessException(ADD_EDGE_BEFORE_VERTEX, bag.edge);
            }
        }
    }
    for (vertex_t v = 0; v < n; v++) {
        if (added[v] != 1) {
            throw NiceBagsCorrectnessException(ADD_VERTEX_E, v);
        }
        if (removed[v] != 1) {
            throw NiceBagsCorrectnessException(REMOVE_VERTEX_E, v);
        }
    }

    for (auto it = boost::edges(_g).first; it != boost::edges(_g).second; it++) {
        std::pair<int, int> edge = std::make_pair(it->m_source, it->m_target);
        if (!edge_used.count(edge)) {
            throw NiceBagsCorrectnessException(MISS_EDGE, *it);
        }
    }
    if (e > _g.m_edges.size()) {
        throw NiceBagsCorrectnessException(EXTRA_EDGE);
    }
}

std::vector<std::vector<vertex_t>> NicePathDecomposition::convert_nice_bags(std::vector<Bag> nice_bags) {
    std::vector<std::vector<vertex_t>> bags;
    std::vector<vertex_t> bag;
    bool new_vertex = false;
    for (Bag nice_bag : nice_bags) {
        if (nice_bag.type == ADD_VERTEX) {
            bag.push_back(nice_bag.vertex);
            new_vertex = true;
        } else if (nice_bag.type == REMOVE_VERTEX) {
            remove(bag.begin(), bag.end(), nice_bag.vertex);
            if (new_vertex) {
                bags.push_back(bag);
                new_vertex = false;
            }
        }
    }
    return bags;
}