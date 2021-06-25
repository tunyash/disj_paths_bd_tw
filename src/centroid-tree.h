#include <vector>
#include <string>
#include "path-decomposition.h"

class CentroidTree {

private:
    std::vector<vertex_t> _parents;
    T _tree;

    void build();

    void check();

public:

    class TreeException : public std::exception {
        /*
         * Thrown when |_tree| is not a tree
         */
    private:

        string msg;
        /*
         * |msg| is used in the |what()| fucntion
         */
    public:

        TreeException();
    };

    PathDecomposition get_path_decomposition() {
        /*
         * @return PathDecomposition of |_tree| with width at most width(_tree) x log(|V(_tree)|)
         */
    }

};