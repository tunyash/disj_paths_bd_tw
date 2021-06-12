#include <vector>
#include <string>
#include <set>
#include <boost/graph/adjacency_list.hpp>
#include "../tdlib/src/treedec_traits.hpp"
#include "../tdlib/src/graph_traits.hpp"

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor vertex_t;
typedef std::set<vertex_t> bag_container_type;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
        boost::property<treedec::bag_t, bag_container_type>> Tree;
typedef boost::graph_traits<Tree>::vertex_descriptor tree_vertex;

class PathDecomposition {
    std::vector<std::vector<vertex_t>> _bags;
    Graph _g;

public:

    enum error_types {
        EDGES = 0,
        NEGATIVE_VERTICES = 1,
        INCONTINUITY = 2,
        VERTICES = 3
    };

    struct CorectnessException : public std::exception{
        /*
        Thrown when path-width decomposition is incorrect
        */
    private:
    public:
        int _error_type;
        int _bag_i, _bag_j, _bag_k, _violating_node;

        CorectnessException(int error_type);
        /*
        @error_type == 0 used when in graph |_g| exists edge (v, u) but in |_bags| there is no bag that contains {v, u}
        @error_type == 1 used when in |_bags| exists vertex |v| that does not belong to [0; |V(_g)|)
        @error_type == 3 used when exists vertex |v| that does not belong to |_bags|
        */
        CorectnessException(int i, int j, int k, int u);
        /*
        This is used only for error_type == 2
        @error_type == 2 used when _bags[i] and _bags[k] contains vertex |v| but _bags[j] does not.
        i < j < k
        */
        ~CorectnessException() throw() {}
        const char* what() const throw();
    };

    PathDecomposition(std::vector<std::vector<vertex_t>> bags, Graph g);
    /*
    Constructor creates path-width decomposition where |bags[i]| is i-th bag of decomposition.
    |bags| is path-width decompostion of graph |g|.
    @exceptions throw CorrectnessException if decomposition is not correct
    */

    explicit PathDecomposition(Graph g);
//    Calls function |transform|
//    @exceptions throw CorrectnessException if algorithm works incorrect

    const std::vector<std::vector<vertex_t>>& get_bags() const {
        return _bags;
    }

private:

    void Check();
    /*
        Function checks correctness of path-width decomposition |_bags| of graph |_g|.
        @exceptions throw CorrectnessException if decomposition is not correct
    */

    void transform();
    // Function creates path-width decomposition of graph |_g| with tdlib library and centroid-decomposition algorithm

};

