#pragma once

class Xor : Gate {
    protected:
        vector<Gate> inputs;

    public:
        void scrub();

        void connect(Gate gate);

        virtual bool determine();
};
