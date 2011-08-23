void Gate::scrub() {
    inputs.clear();
}

void Gate::connect(Gate gate) {
    inputs.add(gate);
}

virtual bool Gate::determine();
