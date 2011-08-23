class Source : Gate{
    boolean on;

    void connect(Gate gate) {
        throw new RuntimeException("Not callable");
    }

    void scrub() {
        throw new RuntimeException("Not callable");
    }

    boolean determine() {
        return on;
    }
};
