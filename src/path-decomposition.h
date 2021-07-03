#ifndef PATH_DECOMPOSITION_H
#define PATH_DECOMPOSITION_H

#include <vector>
#include <string>
#include <boost/graph/adjacency_list.hpp>
#include "../tdlib/src/treedec_traits.hpp"

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor vertex_t;
typedef treedec::graph_traits<Graph>::treedec_type Tree;
typedef boost::graph_traits<Tree>::vertex_descriptor tree_vertex_t;
typedef boost::graph_traits<Tree>::edge_descriptor tree_edge_t;
typedef boost::graph_traits<Graph>::edge_descriptor edge_t;

class PathDecomposition {
protected:
    std::vector<std::vector<vertex_t>> _bags;
    Graph _g;

public:

    enum error_types {
        EDGES = 0,
        NEGATIVE_VERTICES = 1,
        INCONTINUITY = 2,
        VERTICES = 3
    };

    struct CorectnessException : public std::exception {
        /*
        Thrown when path-width decomposition is incorrect
        */
    private:

        std::string msg;

    public:

        int _error_type;
        int _bag_i, _bag_j, _bag_k, _violating_node;
        PathDecomposition *_PD;
        //_PD is PathDecomposition object which caused exception

        CorectnessException(int error_type, PathDecomposition *PD);

        /*
        @error_type == 0 is used when in graph |_g| exists edge (v, u) but in |_bags| there is no bag that contains {v, u}
        @error_type == 1 is used when in |_bags| exists vertex |v| that does not belong to [0; |V(_g)|)
        @error_type == 3 is used when exists vertex |v| that does not belong to |_bags|
        */

        CorectnessException(int bag_i, int bag_j, int bag_k, int violating_node, PathDecomposition *PD);
        /*
        This is used only for error_type == 2
        @error_type == 2 is used when |_bags[bag_i]| and |_bags[bag_k]|
        contains vertex |violating_node| but _bags[bag_k] does not.
        bag_i < bag_j < bag_k
        */
        ~CorectnessException() throw() {}

        const char *what() const throw();
    };

    PathDecomposition(std::vector<std::vector<vertex_t>> &bags, Graph &g);

    /*
    Constructor creates path-width decomposition where |bags[i]| is i-th bag of decomposition.
    |bags| is path-width decompostion of graph |g|.
    @exceptions throw CorrectnessException if decomposition is not correct
    */

    PathDecomposition() = default;
//

    const std::vector<std::vector<vertex_t>> &get_bags() const {
        return _bags;
    }

    const Graph &get_graph() const {
        return _g;
    }

    void enumerate(std::vector<vertex_t> &U);
    /*
     * This function sorts U and assign and change vertex |i| to vertex |U[i]|
     * Check out fucntion in composed-graph.h get_good_subgraph
     * Also check composed-test.cpp to
     * It is not recommended to use this function without get_good_subgraph
     */

protected:

    void check();

    /*
        Function checks correctness of path-width decomposition |_bags| of graph |_g|.
        @exceptions throw CorrectnessException if decomposition is not correct
    */

};

#endif
