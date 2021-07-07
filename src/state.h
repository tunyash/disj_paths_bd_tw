#ifndef STATE_H
#define STATE_H

#include "nice-path-decomposition.h"

struct Parameters {
    Parameters(std::vector<vertex_t> s, std::vector<vertex_t> t, int n);

    std::vector<vertex_t> s, t;
    std::vector<int> color;
    // color[v] = x ∈ [-1,|s|) : color[s[i]] = i, color[t[i]] = i, other color[v] = -1
};

class State {
    std::set<vertex_t> available;
    // ends of paths or vertices out from paths

    std::map<vertex_t, int> end;
    // keeps another end of path;

    const Parameters parameters;

public:
    int complete_paths = 0;

    std::vector<State> calculate(const NicePathDecomposition::Bag &bag);
    // returns states in which we can go with this bag

    State(std::set<vertex_t> available, std::map<vertex_t, int> color, int complete_paths,
          const Parameters &parameters);

    vertex_t phi(vertex_t v);
    // (end of path) -> (another end of path)

};


#endif 