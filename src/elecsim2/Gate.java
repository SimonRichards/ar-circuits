package elecsim2;

import java.util.Collection;
import java.util.LinkedList;

abstract class Gate {


    protected Collection<Gate> inputs = new LinkedList<>();

    void scrub() {
        inputs.clear();
    }

    void connect(Gate gate) {
        inputs.add(gate);
    }

    abstract boolean determine();
}
