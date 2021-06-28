#include "centroid-tree.h"


class NicePathDecomposition : public PathDecomposition {
    /*
        NicePathDecomposition is a path decomposition but every bag has one of three types: ADD_VERTEX, REMOVE_VERTEX, ADD_EDGE.
        The purpose of using it is that it will help to calculate the dynamic.
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
        // Constructor for ADD_VERTEX and REMOVE_VERTEX bags

        Bag(bag_types bag_type, edge_t edge) : type(bag_type), edge(edge) {}
        // Constructor for ADD_EDGE bags

        Bag() = default;

        bool operator<(Bag b);
        // Use it to sort bags
    };

    enum error_types {
        MISS_VERTEX, MISS_EDGE, LEFT_VERTEX
    };

    struct NiceBagsCorrectnessException : public std::exception {
        // thrown if NicePathDecomposition is incorrect
    private:
        std::string msg;
    public:
        error_types type;

        NiceBagsCorrectnessException(error_types type, vertex_t v);

        NiceBagsCorrectnessException(edge_t edge);

        ~NiceBagsCorrectnessException() throw() = default;

        const char *what() const throw();
    };


    const std::vector<Bag> &get_nice_bags();

    NicePathDecomposition(std::vector<std::vector<vertex_t>> &bags, Graph &g);
    // Constructor makes NicePathDecomposition using bags sequence

    explicit NicePathDecomposition(PathDecomposition pathDecomposition);
    // Constructor makes NicePathDecomposition using PathDecomposition

    void is_correct();
    // Function checks if _nice_bags is correct

private:
    std::vector<Bag> _nice_bags;

    void set_g(Graph &g);

    void fill_nice_bags();
};