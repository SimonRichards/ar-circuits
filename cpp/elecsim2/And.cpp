bool And::determine() {
    for (Gate input : inputs) {
        if (!input.determine()) {
            return false;
        }
    }
    return true;
}
