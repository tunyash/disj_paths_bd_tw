#include <iostream>
#include "tdlib/src/combinations.hpp"
#include <boost/graph/adjacency_list.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> Graph;
typedef treedec::graph_traits<Graph>::treedec_type Tree;

int main() {
    int n = 4;
    std::vector<int> X = {1, 0};
    std::vector<int> Y = {0, 1};
    std::vector<std::pair<int, int>> edges;

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            for (int go = 0; go < 2; ++go) {
                if (i + X[go] < n && j + Y[go] < n) {
                    int v = i * n + j, u = (i + X[go]) * n + j + Y[go];
                    edges.push_back(std::make_pair(v, u));
                }
            }
        }
    }


    Graph g(edges.begin(), edges.end(), n * n);
    Tree t;
    treedec::comb::PP_FI<Graph> algo(g);
    algo.do_it();
    algo.get_tree_decomposition(t);
}
