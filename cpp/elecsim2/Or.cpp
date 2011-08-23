bool Or::determine() {
    for (Gate input : inputs) {
        if (input.determine()) {
            return true;
        }
    }
    return false;
}
