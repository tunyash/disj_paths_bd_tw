#ifndef STATE_H
#define STATE_H

#include "nice-path-decomposition.h"

class State {
    int n;
    // size of graph
    std::set<vertex_t> available;
    // ends of paths or vertices out from paths
    std::map<vertex_t,vertex_t> phi;
    // (end of path) -> (another end of path)
public:
    std::vector<State> calculate(NicePathDecomposition::Bag bag);
    // returns states in which we can go with this bag
    State(std::set<vertex_t>available,std::map<vertex_t,vertex_t>)
};


#endif 