#pragma once

class Sink {
    private:
        Gate input;

        public;
        bool determine() {
            return input.determine();
        }

        void connect(Gate input) {
            this.input = input;
        }
};
