#include "state.h"

Parameters::Parameters(std::vector<vertex_t> s, std::vector<vertex_t> t, int n) : s(s), t(t) {
    color.resize(n, -1);
    for (int i = 0; i < s.size(); i++)
        color[s[i]] = i, color[s[t[i]]] = i;
}

vertex_t State::phi(vertex_t v) {
    if (available.count(v)) return end[v];
    return -1;
}

State::State(std::set<vertex_t> available, std::map<vertex_t, int> end, int complete_paths,
             const Parameters &parameters) :
        available(available),
        end(end),
        complete_paths(complete_paths),
        parameters(parameters) {}


std::vector<State> State::calculate(const NicePathDecomposition::Bag &bag) {
    std::set<vertex_t> new_available = available;
    std::map<vertex_t, int> new_end = end;
    switch (bag.type) {
        case NicePathDecomposition::ADD_VERTEX:
            new_available.insert(bag.vertex);
            new_end[bag.vertex] = bag.vertex;
            return {State(new_available, new_end, complete_paths, parameters)};
        case NicePathDecomposition::REMOVE_VERTEX:
            new_available.erase(bag.vertex);
            return {State(new_available, new_end, complete_paths, parameters)};
        case NicePathDecomposition::ADD_EDGE:
            std::vector<State> states = {State(new_available, new_end, complete_paths, parameters)};
            vertex_t v = bag.edge.m_source, u = bag.edge.m_target;
            if (!available.count(v) || !available.count(u))
                return states;
            vertex_t end1 = phi(v), end2 = phi(u);
            bool complete_path = false;
            if (parameters.color[end1] != -1 && parameters.color[end2] != -1) {
                if (parameters.color[end1] != parameters.color[end2]) return states;
                complete_path = true;
            }
            if (end1 != v || parameters.color[v] != -1) new_available.erase(v);
            if (end2 != u || parameters.color[u] != -1) new_available.erase(u);
            new_end[end1] = end2, new_end[end2] = end1;
            states.emplace_back(new_available, new_end, complete_path + complete_path, parameters);
            return states;
    }
}
