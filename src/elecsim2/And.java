package elecsim2;


class And extends Gate{

    @Override
    boolean determine() {
        for (Gate input : inputs) {
            if (!input.determine()) {
                return false;
            }
        }
        return true;
    }


}
