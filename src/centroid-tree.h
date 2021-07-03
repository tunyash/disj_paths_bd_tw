#ifndef CENTROID_TREE_H
#define CENTROID_TREE_H

#include <vector>
#include <string>
#include "path-decomposition.h"
#include <boost/graph/depth_first_search.hpp>

class CentroidTree {

private:
    std::vector<vertex_t> _parents;
    Tree _tree;
    std::vector<vertex_t> _order;

    void build();
    /*
     * Building centroid tree
     */

    void check();
    /*
     * Function checks that graph |_tree| is tree
     * @exception TreeException is thrown when graph |_tree| is not a tree
     * */

public:

    class TreeException : public std::exception {
    /*
     * Thrown when |_tree| is not a tree
     */

    public:

        Tree &_t;

        explicit TreeException(Tree &t);
        /*
         * @param |t| is graph where a loop was found
         */

        const char * what() const throw();
    };

    explicit CentroidTree(Tree &t);
    /*
     * @param |t| is tree-width decomposition of some graph |g|
     * P.S.: same graph |g| is used in the get_path_decomposition() function
     */

    explicit CentroidTree(Graph &g);
    /*
     * This fucntion uses CentroidTree(Tree &t)
     * @param |t| is received from |g| by exact_decomposition_cutset algo from tdlib
     */

    PathDecomposition get_path_decomposition(Graph &g);
    /*
    * @return PathDecomposition of |g| with width at most width(_tree) * log(|V(_tree)|)
    */

};

#endif
