#pragma once
#include "Logic.hpp"
using namespace std;

class Source : public Gate{
    private:

    public:
        bool on;

        void connect(Gate* gate);

        void scrub();

        virtual bool determine();
};
