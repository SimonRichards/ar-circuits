class And : Gate{

    boolean determine() {
        for (Gate input : inputs) {
            if (!input.determine()) {
                return false;
            }
        }
        return true;
    }
};
