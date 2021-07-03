#include "path-decomposition.h"
#include "nice-path-decomposition.h"
#include "composed-graph.h"

std::vector<NicePathDecomposition> get_path_dec_U(Graph &g, std::vector<std::vector<vertex_t>> &U) {
    std::vector<NicePathDecomposition> res;
    for (auto _set : U) {
        Graph ng = get_good_subgraph(g, _set);
        CentroidTree CT(ng);
        auto PD = CT.get_path_decomposition(ng);
        res.push_back(NicePathDecomposition(PD));
        res.back().enumerate(_set);
    }
    return res;
}

NicePathDecomposition get_path_dec_comp(ComposedGraph &CG) {
    Graph compressed = CG.get_compressed_graph();
    CentroidTree CT(compressed);
    return NicePathDecomposition(CT.get_path_decomposition(compressed));
}

NicePathDecomposition perform_path_dec(Graph &g, std::vector<std::vector<vertex_t>> &U) {
    auto pw_u = get_path_dec_U(g, U);
    ComposedGraph CG(g, U, pw_u);
    auto pw_comp = get_path_dec_comp(CG);
    return CG.get_path_decomposition(pw_comp);
}

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
    _msg = "|_G.U[_U_i]| and |_G.U[_U_j]| have common vertex |_common_vertex|";
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

NicePathDecomposition ComposedGraph::get_path_decomposition(NicePathDecomposition &pw_compressed) {
    int n = boost::num_vertices(_G);
    _pw_compressed = pw_compressed;

    std::vector<bool> used(boost::num_vertices(_G), false);
    std::vector<NicePathDecomposition::Bag> G_path_decomp;

    for (auto bag : _pw_compressed.get_nice_bags()) {
        if (bag.type != NicePathDecomposition::ADD_VERTEX) continue;
        for (auto in_bag : _pw_u[bag.vertex].get_nice_bags()) {
            if (in_bag.type == NicePathDecomposition::ADD_VERTEX) {
                if (used[in_bag.vertex]) continue;
                G_path_decomp.push_back(NicePathDecomposition::Bag
                                                (in_bag.type, in_bag.vertex));

                used[in_bag.vertex] = true;

                if (_matchings[in_bag.vertex] >= n) continue;
                G_path_decomp.push_back(NicePathDecomposition::Bag
                                                (in_bag.type, _matchings[in_bag.vertex]));

                used[_matchings[in_bag.vertex]] = true;

                G_path_decomp.push_back(NicePathDecomposition::Bag
                                                (boost::edge(in_bag.vertex, _matchings[in_bag.vertex], _G).first));
            } else {
                G_path_decomp.push_back(in_bag);
            }
        }
    }

    return NicePathDecomposition(G_path_decomp, _G);
}

ComposedGraph::ComposedGraph(Graph &G,
                             std::vector<std::vector<vertex_t>> &U,
                             std::vector<NicePathDecomposition> &pw_u) :
                             _pw_u(pw_u), _G(G) {
    for (auto &vec : _U) {
        std::sort(vec.begin(), vec.end());
        vec.resize(std::unique(vec.begin(), vec.end()) - vec.begin());
    }
    _U = U;
    check();
}

Graph ComposedGraph::get_compressed_graph() {
    Graph __G__(_U.size());
    for (auto item = boost::edges(_G).first; item != boost::edges(_G).second; ++item) {
        if (_id_U[boost::source(*item, _G)] == _id_U[boost::target(*item, _G)]) continue;
        if (boost::edge(_id_U[boost::source(*item, _G)], _id_U[boost::target(*item, _G)], __G__).second) continue;
        boost::add_edge(_id_U[boost::source(*item, _G)], _id_U[boost::target(*item, _G)], __G__);
    }
    return __G__;
}

Graph get_good_subgraph(Graph &g, std::vector<vertex_t> &U) {
    std::sort(U.begin(), U.end());
    U.resize(std::unique(U.begin(), U.end()) - U.begin());
    if (U.back() > boost::num_vertices(g)) throw std::out_of_range("Too big vertices");

    Graph res(U.size());

    for (auto item = boost::edges(g).first; item != boost::edges(g).second; ++item) {
        int pos_src = std::lower_bound(U.begin(), U.end(), boost::source(*item, g)) - U.begin();
        int pos_trg = std::lower_bound(U.begin(), U.end(), boost::target(*item, g)) - U.begin();
        if (pos_src == U.size() || pos_trg == U.size()) continue;
        if (U[pos_src] != boost::source(*item, g) || U[pos_trg] != boost::target(*item, g)) continue;
        boost::add_edge(pos_src, pos_trg, res);
    }

    return res;
}