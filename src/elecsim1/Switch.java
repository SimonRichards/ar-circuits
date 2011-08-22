
package elecsim1;

class Switch extends Component {
    boolean closed;

    Switch(boolean closed) {
        this.closed = closed;
    }

    Switch() {

    }


    @Override
    double getResistance() {
        return closed ? 0 : Double.POSITIVE_INFINITY;
    }
}
