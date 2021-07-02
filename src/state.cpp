#include "state.h"


std::vector<State> State::calculate(NicePathDecomposition::Bag bag){
    switch (bag.type) {
        case NicePathDecomposition::ADD_VERTEX:
            available.insert(bag.vertex);
            break;
        case NicePathDecomposition::REMOVE_VERTEX:
            break;
        case NicePathDecomposition::ADD_EDGE:
            break;
    }
}