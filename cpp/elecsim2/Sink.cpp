class Sink {

    Gate input;

    bool determine() {
        return input.determine();
    }

    void connect(Gate input) {
        this.input = input;
    }
};
