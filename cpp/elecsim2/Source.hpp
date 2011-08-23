#pragma once

class Source : Gate{
    private:
        bool on;

    public:

        void connect(Gate gate);

        void scrub();

        bool determine();
};
