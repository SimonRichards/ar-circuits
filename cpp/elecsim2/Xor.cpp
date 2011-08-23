class Xor : Gate{

    boolean determine() {
        int count = 0;
        for (Gate input : inputs) {
            if (input.determine()) {
                count++;
            }
        }
        return count == 1;
    }

};
