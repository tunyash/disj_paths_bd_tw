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
        vertex_t vertex = -1;
        edge_t edge;

        Bag(bag_types bag_type, vertex_t v) : type(bag_type), vertex(v) {}
        // Constructor for ADD_VERTEX and REMOVE_VERTEX bags

        Bag(edge_t edge) : type(ADD_EDGE), edge(edge) {}
        // Constructor for ADD_EDGE bags

        Bag() = default;

        bool operator<(Bag b);
        // Use it to sort bags
    };

    enum error_types {
        ADD_VERTEX_E, REMOVE_VERTEX_E, REMOVE_BEFORE_ADDING, ADD_EDGE_BEFORE_VERTEX, MISS_EDGE, EXTRA_EDGE,
    };

    struct NiceBagsCorrectnessException : public std::exception {
        // thrown if NicePathDecomposition is incorrect
    private:
        std::string msg;
    public:
        error_types type;

        NiceBagsCorrectnessException(error_types type, vertex_t v);

        NiceBagsCorrectnessException(error_types type, edge_t edge);

        NiceBagsCorrectnessException(error_types type);

        ~NiceBagsCorrectnessException() throw() = default;

        const char *what() const throw();
    };


    const std::vector<Bag> &get_nice_bags();

    NicePathDecomposition(std::vector<Bag> &nice_bags, Graph &g);

    NicePathDecomposition(std::vector<std::vector<vertex_t>> &bags, Graph &g);
    // Constructor makes NicePathDecomposition using bags sequence

    explicit NicePathDecomposition(PathDecomposition pathDecomposition);
    // Constructor makes NicePathDecomposition using PathDecomposition

    static std::vector<std::vector<vertex_t>> convert_nice_bags(std::vector<Bag> nice_bags);
    // Convert nice_bag to bag sequence

    void is_correct();
    // Function checks if _nice_bags is correct

private:
    std::vector<Bag> _nice_bags;

    void set_g(Graph &g);

    void fill_nice_bags();
};