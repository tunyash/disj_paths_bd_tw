#include "nice-path-decomposition.h"
#include "path-decomposition.h"
#include <string>
#include <vector>

class ComposedGraph {
    /*
     * Def: ComposedGraph is a graph G that has a set U of sets of vertices of G.
     * Def: edge (v, u) is OUTER if v is in U[i] and u is in U[j] and i != j
     * Rule: Union of sets U[i] = V(G) for i = 0...|V| - 1
     * Rule: Intersection of U[i] and U[j] is empty for all i != j
     * Rule: Vertex is adjacent to 0 or 1 outer edges
     *
     * Def: Compressed graph is a graph __G__ formed from graph G
     * Def: __G__ has |U| vertices
     * Def: Edge (i, j) is in __G__ if in G exists outer edge (v, u) such that v in U[i] and u in U[j]
     *
     * Purpose of this class is to create fast way to provide path-width decomposition of a big graph
     */

public:

    ComposedGraph(Graph &G, std::vector<std::vector<vertex_t>> &U,
                  std::vector<NicePathDecomposition> &pw_u,
                  NicePathDecomposition &pw_compressed);
    /*
     * @param |G| is graph G in the description
     * @param |U| is set U in the description
     * @param |pw_for_u| is a set of NicePathDecomposition
     * |pw_u[i]| is NicePathDecomposition of subgraph G[U[i]]
     * |pw_comp_graph| is NicePathDecomposition of compressed graph __G__
     */

    NicePathDecomposition get_path_decomposition();
    /*
     * @return NicePathDecomposition of graph G
     */

private:

    class CorrectnessException : public std::exception {

        /*
         * This class has ComposedGraph _G which caused this exception
         */

    public:

        CorrectnessException(vertex_t outer_vertex, ComposedGraph *G);

        /*
         * This constructor is used for INCOMPLETE_UNION error.
         * outer_vertex does not belong to any U[i]
         * That conflicts with RULE above
         */

        CorrectnessException(ComposedGraph *G,
                             int U_i, int U_j,
                             vertex_t common_vertex);

        /*
         * This constructor is used for NON_EMPTY_INTERSECTION error.
         * U[U_i] and U[U_j] have common vertex |common_vertex|.
         * That conflicts with RULE above
         */

        CorrectnessException(vertex_t matching_vertex,
                             vertex_t adjacent_vertex_first,
                             vertex_t adjacent_vertex_second,
                             ComposedGraph *G);

        /*
         * This constructor is used for NOT_MATCHING error
         * Vertex matching_vertex has at least 2 adjacent vertices
         * 2 of them are called |_adjacent_vertex_first| and |_adjacent_vertex_second|
         * That conflicts with RULE above
         */

        const char *what() const throw();

        ComposedGraph *_G;
        int _U_i, _U_j;
        vertex_t _matching_vertex;
        vertex_t _adjacent_vertex_first, _adjacent_vertex_second;
        vertex_t _common_vertex;
        vertex_t _outer_vertex;

    private:

        std::string _msg;

    };

    void check();

    std::vector<vertex_t> _matchings;
    std::vector<vertex_t> _id_U;
    Graph _G;
    std::vector<std::vector<int>> _U;
    std::vector<NicePathDecomposition> _pw_u;
    NicePathDecomposition _pw_compressed;

};
