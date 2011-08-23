#pragma once gate_h

virtual class Gate {
    protected:
        vector<Gate> inputs;

    public:
        void scrub();

        void connect(Gate gate);

        virtual bool determine();
};
