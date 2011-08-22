package elecsim2;


class Sink {

    Gate input;

    boolean determine() {
        return input.determine();
    }

    void connect(Gate input) {
        this.input = input;
    }

}
