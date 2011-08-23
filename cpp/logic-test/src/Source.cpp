#include "Logic.hpp"
using namespace std;


//void Source::connect(Gate gate) {
//    throw new RuntimeException("Not callable");
//}
//
//void Source::scrub() {
//    throw new RuntimeException("Not callable");
//}

bool Source::determine() {
    return on;
}