#include "Logic.hpp"
using namespace std;

bool Or::determine() {
    for (unsigned int i = 0; i < _inputs.size(); i++) {
        if (_inputs[i]->determine()) {
            return true;
        }
    }
    return false;
}
