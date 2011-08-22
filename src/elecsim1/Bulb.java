package elecsim1;

class Bulb extends Component{
    private double resistance;
    private static double DEFAULT_RESISTANCE = 1;

    Bulb(double resistance) {
        this.resistance = resistance;
    }

    Bulb() {
        this(DEFAULT_RESISTANCE);
    }

    double getPower() {
        return Math.pow(getLeft().voltage - getRight().voltage,2) / resistance;
    }

    @Override
    double getResistance() {
        return resistance;
    }


}
