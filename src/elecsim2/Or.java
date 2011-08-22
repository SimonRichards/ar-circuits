package elecsim2;


class Or extends Gate{

    @Override
    boolean determine() {
        for (Gate input : inputs) {
            if (input.determine()) {
                return true;
            }
        }
        return false;
    }

}
