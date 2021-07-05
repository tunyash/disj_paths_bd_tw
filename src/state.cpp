#include "state.h"

vertex_t State::phi(vertex_t v) {
    if (available.count(v)) return color[v];
    return -1;
}

State::State(std::set<vertex_t> available, std::map<vertex_t, int> color) : available(available),
                                                                            color(color) {}

std::vector<State> State::calculate(NicePathDecomposition::Bag bag) {
    std::set<vertex_t> new_available = available;
    std::map<vertex_t, int> new_color = color;
    switch (bag.type) {
        case NicePathDecomposition::ADD_VERTEX:
            // just add vertex to available
            new_available.insert(bag.vertex);
            new_color[bag.vertex] = bag.vertex;
            return {State(new_available, new_color)};
        case NicePathDecomposition::REMOVE_VERTEX:
            // delete vertex from available
            new_available.erase(bag.vertex);
            return {State(new_available, new_color)};
        case NicePathDecomposition::ADD_EDGE:
            std::vector<State> states = {State(new_available, new_color)};
            vertex_t v = bag.edge.m_source, u = bag.edge.m_target;
            // if one of vertexes is unavailable we can't change state
            if (!available.count(v) || !available.count(u))
                return states;
            vertex_t end1 = phi(v), end2 = phi(u);
            if (end1 != v) new_available.erase(v);
            if (end2 != u) new_available.erase(u);
            new_color[end1] = end2, new_color[end2] = end1;
            states.push_back(State(new_available, new_color));
            return states;
    }
}