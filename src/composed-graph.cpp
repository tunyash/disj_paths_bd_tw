#include "path-decomposition.h"
#include "nice-path-decomposition.h"
#include "composed-graph.h"

void ComposedGraph::check() {
    int n = boost::num_vertices(_G);
    _matchings.assign(n, -1);
    _id_U.assign(n, -1);

    for (int i = 0; i < _U.size(); ++i) {
        for (vertex_t v : _U[i]) {
            if (_id_U[v] < n) throw CorrectnessException(this, _id_U[v], i, v);
            _id_U[v] = i;
        }
    }

    for (vertex_t v = 0; v < n; ++v) {
        if (_id_U[v] >= n) throw CorrectnessException(v, this);
    }

    for (auto it = boost::edges(_G).first; it != boost::edges(_G).second; ++it) {
        edge_t e = *it;
        vertex_t v = boost::source(e, _G);
        vertex_t u = boost::target(e, _G);
        if (_id_U[v] == _id_U[u]) continue;
        if (_matchings[v] < n) throw CorrectnessException(v, _matchings[v], u, this);
        if (_matchings[u] < n) throw CorrectnessException(u, _matchings[u], v, this);
        _matchings[u] = v;
        _matchings[v] = u;
    }
}

ComposedGraph::CorrectnessException::CorrectnessException(vertex_t outer_vertex, ComposedGraph *G):
_G(G), _outer_vertex(outer_vertex) {
    _msg = "In ComposedGraph |_G| vertex |_outer_vertex| does not belongs to any U[i]";
}

ComposedGraph::CorrectnessException::CorrectnessException(ComposedGraph *G, int U_i, int U_j, vertex_t common_vertex):
_G(G), _U_i(U_i), _U_j(U_j), _common_vertex(common_vertex) {
    _msg = "|U[_U_i]| and |U[_U_j]| have common vertex |_common_vertex|";
}

ComposedGraph::CorrectnessException::CorrectnessException(vertex_t matching_vertex,
                                  vertex_t adjacent_vertex_first,
                                  vertex_t adjacent_vertex_second,
                                  ComposedGraph *G):
                                  _matching_vertex(matching_vertex),
                                  _adjacent_vertex_first(adjacent_vertex_first),
                                  _adjacent_vertex_second(adjacent_vertex_second) {
    _msg = std::string("Vertex |_matching_vertex| has 2 outer edges ") +
            std::string("(_matching_vertex, _adjacent_vertex_first) and ") +
            std::string("(_matching_vertex, _adjacent_vertex_second)");
}

const char * ComposedGraph::CorrectnessException::what() const throw() {
    return _msg.c_str();
}

NicePathDecomposition ComposedGraph::get_path_decomposition() {
    std::vector<bool> used(boost::num_vertices(_G), false);
    std::vector<NicePathDecomposition::Bag> G_path_decomp;

    for (auto bag : _pw_compressed.get_nice_bags()) {
        if (bag.type != NicePathDecomposition::ADD_VERTEX) continue;
        for (auto in_bag : _pw_u[bag.vertex].get_nice_bags()) {
            if (in_bag.type == NicePathDecomposition::ADD_VERTEX) {
                if (used[in_bag.vertex]) continue;
                G_path_decomp.push_back(NicePathDecomposition::Bag
                (in_bag.type, in_bag.vertex));

                G_path_decomp.push_back(NicePathDecomposition::Bag
                (in_bag.type, _matchings[in_bag.vertex]));

                G_path_decomp.push_back(NicePathDecomposition::Bag
                (boost::edge(in_bag.vertex, _matchings[in_bag.vertex], _G).first));
            }
            if (in_bag.type == NicePathDecomposition::ADD_EDGE) {
                G_path_decomp.push_back(in_bag);
            }
        }
    }

    return NicePathDecomposition(G_path_decomp, _G);
}