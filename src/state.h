#ifndef STATE_H
#define STATE_H

#include "nice-path-decomposition.h"

class State {
    std::set<vertex_t> available;
    // ends of paths or vertices out from paths

    std::map<vertex_t,int> color;
    // color helps to find phi;

public:
    std::vector<State> calculate(NicePathDecomposition::Bag bag);
    // returns states in which we can go with this bag

    State(std::set<vertex_t>available,std::map<vertex_t,int> color);

    vertex_t phi(vertex_t v);
    // (end of path) -> (another end of path)
};


#endif 