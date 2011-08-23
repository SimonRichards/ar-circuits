class Source : Gate{
    bool on;

    void connect(Gate gate) {
        throw new RuntimeException("Not callable");
    }

    void scrub() {
        throw new RuntimeException("Not callable");
    }

    bool determine() {
        return on;
    }
};
