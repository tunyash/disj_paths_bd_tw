#include "nice-path-decomposition.h"
//#include "../centroid-tree.h"

int main(){
    int n = 7;
    std::vector<std::pair<int, int>> edges = {
            {0, 1},
            {0, 2},
            {1, 3},
            {1, 4},
            {2, 5},
            {2, 6}
    };

    Graph g(edges.begin(), edges.end(), n);
    CentroidTree c(g);
    NicePathDecomposition p(c.get_path_decomposition(g));

    auto a = p.get_bags();
    auto nice_a = p.get_nice_bags();

    return 0;
}