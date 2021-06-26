#include "../path-decomposition.h"

class NicePathDecomposition : public PathDecomposition {
public:
    enum bag_types {
        ADD_VERTEX, REMOVE_VERTEX, ADD_EDGE
    };

    struct Bag {
        bag_types bag_type;
        std::vector<vertex_t> vertices;
        vertex_t v, u;

        Bag(bag_types bag_type, std::vector<vertex_t> vertices, vertex_t v, vertex_t u) : bag_type(bag_type),
                                                                                          vertices(vertices), v(v),
                                                                                          u(u) {}
    };

    std::vector<Bag> get_nice_bags();

    NicePathDecomposition(std::vector<std::vector<vertex_t>> &bags, Graph &g);

    NicePathDecomposition(Graph &g);

    NicePathDecomposition(PathDecomposition pathDecomposition);

private:
    std::vector<Bag> _nice_bags;

    void set_g(Graph &g);

    void nice_transform();

};