#include "centroid-tree.h"


class NicePathDecomposition : public PathDecomposition {
    /*
        NicePathDecomposition is a path decomposition but every bag has one of three types: ADD_VERTEX, REMOVE_VERTEX, ADD_EDGE.
        The porpoise of using it is that it will help to calculate the dynamic.
    */
public:
    enum bag_types {
        ADD_VERTEX, REMOVE_VERTEX, ADD_EDGE
    };

    struct Bag {
        bag_types type;
        vertex_t v = -1;
        edge_t edge;

        Bag(bag_types bag_type, vertex_t v) : type(bag_type), v(v) {}

        Bag(bag_types bag_type, edge_t edge) : type(bag_type), edge(edge) {}

        Bag() = default;

        bool operator<(Bag b);
        // Use it to sort bags
    };


    std::vector<Bag> get_nice_bags();

    NicePathDecomposition(std::vector<std::vector<vertex_t>> &bags, Graph &g);
    // Constructor makes NicePathDecomposition using bags sequence

    explicit NicePathDecomposition(PathDecomposition pathDecomposition);
    // Constructor makes NicePathDecomposition using PathDecomposition

    bool is_correct();
    // Function checks is _nice_bags is correct

private:
    std::vector<Bag> _nice_bags;

    void set_g(Graph &g);

    void fill_nice_bags();
};