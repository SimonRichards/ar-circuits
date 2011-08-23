#pragma once 
//#include "Logic.hpp"
using namespace std;

class Gate {
    protected:
        std::vector<Gate> _inputs;

    public:
        void scrub();

        void connect(Gate gate);

        virtual bool determine();
};
