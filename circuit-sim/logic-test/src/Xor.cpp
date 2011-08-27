#include "Logic.hpp"
using namespace std;

bool Xor::determine() {
    int count = 0;
    for (unsigned int i = 0; i < _inputs.size(); i++) {
        if (_inputs[i]->determine()) {
            count++;
        }
    }
    return count == 1;
}
