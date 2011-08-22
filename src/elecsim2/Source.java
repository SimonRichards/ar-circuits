package elecsim2;

class Source extends Gate{
    boolean on;

    @Override
    void connect(Gate gate) {
        throw new RuntimeException("Not callable");
    }

    @Override
    void scrub() {
        throw new RuntimeException("Not callable");
    }

    @Override
    boolean determine() {
        return on;
    }



}
