#include "centroid-tree.h"
#include <boost/graph/depth_first_search.hpp>
#include "../tdlib/src/combinations.hpp"


CentroidTree::TreeException::TreeException(Tree &t): _t(t) {}

const char * CentroidTree::TreeException::what() const throw() {
    return "Graph |_t| is not a tree";
}

void CentroidTree::check() {

    struct cycle_detector : public boost::dfs_visitor<> {
        bool _has_cycle = false;
        int _visited = 0;

        void back_edge(const tree_edge_t &e, const Tree &g) {
            _has_cycle = true;
        }

        void discover_vertex(const tree_vertex_t &v, const Tree &g) {
            _visited++;
        }
    };

    cycle_detector vis;
    const Tree &t = _tree;
    boost::depth_first_search(t, visitor(vis));

    if (vis._has_cycle) throw TreeException(_tree);
    if (vis._visited != boost::num_vertices(_tree)) throw TreeException(_tree);
}

void CentroidTree::build() {

    int log_n = 0, n = boost::num_vertices(_tree);
    while ((1 << log_n) < n) ++log_n;
    std::vector<std::vector<int>> size(n, std::vector<int>(log_n + 2));
    // size[v][d] = size of subtree of vertex |v| on depth |d|
    std::vector<int> used(n);
    // used[v] = 1 if vertex |v| is already in centroid tree
    // used[v] = 0 else

    std::function<void(int, int, int)> size_calculation;
    size_calculation = [&](tree_vertex_t v, int depth, tree_vertex_t p) {
        size[v][depth] = 1;
        for (auto item = boost::adjacent_vertices(v, _tree).first;
             item != boost::adjacent_vertices(v, _tree).second;
             ++item) {
            tree_vertex_t u = *item;
            if (p == u || used[u]) continue;
            size_calculation(u, depth, v);
            size[v][depth] += size[u][depth];
        }
    };

    std::function<tree_vertex_t(int, int, int, int)> find_centroid;
    find_centroid = [&](tree_vertex_t v, int depth, int all, tree_vertex_t p) {
        for (auto item = boost::adjacent_vertices(v, _tree).first;
             item != boost::adjacent_vertices(v, _tree).second;
             ++item) {
            vertex_t u = *item;
            if (p == u || used[u]) continue;
            if (size[u][depth] > all / 2) return find_centroid(u, depth, all, v);
        }
        return v;
    };

    std::function<void(int, int, int)> build_cetroid_tree;
    build_cetroid_tree = [&](tree_vertex_t v, int depth, tree_vertex_t p) {
        size_calculation(v, depth, -1);
        tree_vertex_t c = find_centroid(v, depth, size[v][depth], -1);
        used[c] = 1;
        _parents[c] = p;
        int adjacency = 0;

        for (auto item = boost::adjacent_vertices(c, _tree).first;
             item != boost::adjacent_vertices(c, _tree).second;
             ++item) {
            tree_vertex_t u = *item;
            if (used[u]) continue;
            adjacency++;
            build_cetroid_tree(u, depth + 1, c);
        }

        if (adjacency == 0) {
            _order.push_back(c);
        }
    };

    build_cetroid_tree(0, 0, -1);
}

PathDecomposition CentroidTree::get_path_decomposition(Graph &g) {
    int n = boost::num_vertices(_tree);
    std::vector<bool> used(boost::num_vertices(_tree));
    auto tree_bags = get(treedec::bag_t(), _tree);
    std::vector<std::vector<vertex_t>> bags;

    for (vertex_t v : _order) {
        bags.push_back({});
        for (vertex_t u = v; u < n; u = _parents[u]) {
            for (vertex_t i : get(tree_bags, u)) bags.back().push_back(i);
        }
    }

    return PathDecomposition(bags, g);
}

CentroidTree::CentroidTree(Graph &g) {
    Tree t;
    treedec::exact_decomposition_cutset(g, t);
    *this = CentroidTree(t);
}

CentroidTree::CentroidTree(Tree &t): _tree(t) {
    _parents.assign(boost::num_vertices(t), -1);
    check();
    build();
}
