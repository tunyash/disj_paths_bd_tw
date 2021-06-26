#include "../centroid-tree.h"


class NicePathDecomposition : public PathDecomposition {
public:
    enum bag_types {
        ADD_VERTEX, REMOVE_VERTEX, ADD_EDGE
    };

    struct Bag {
        bag_types type;
        vertex_t v;
        edge_t edge;

        Bag(bag_types bag_type, vertex_t v) : type(bag_type), v(v) {}

        Bag(bag_types bag_type, edge_t edge) : type(bag_type), edge(edge) {}
    };

    std::vector<Bag> get_nice_bags();

    NicePathDecomposition(std::vector<std::vector<vertex_t>> &bags, Graph &g);

    explicit NicePathDecomposition(PathDecomposition pathDecomposition);

private:
    std::vector<Bag> _nice_bags;

    void set_g(Graph &g);

    void nice_transform();

};