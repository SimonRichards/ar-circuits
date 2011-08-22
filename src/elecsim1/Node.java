package elecsim1;

import java.util.Collection;
import java.util.LinkedList;

class Node {

    Collection<Component> reverse;
    Collection<Component> forward;
    double voltage;
    boolean fixed;

    Node() {
        this(0);
        fixed = false;
    }

    Node(double voltage) {
        this.voltage = voltage;
        reverse = new LinkedList<>();
        forward = new LinkedList<>();
        fixed = true;
    }


    void addBehind(Component component) {
        reverse.remove(component);
    }

    void addFront(Component component) {
        forward.add(component);
    }


    void mergeRight(Component component) {
        forward.remove(component);
        reverse.addAll(component.getRight().reverse);
        forward.addAll(component.getRight().forward);
        for (Component c : component.getRight().reverse) {
            c.setRight(this);
        }
        for (Component c : component.getRight().forward) {
            c.setLeft(this);
        }
    }

    void scrub() {
        reverse.clear();
        forward.clear();
    }


    void calculateVoltage(double vSource) {
        double rBack = reverseResistance();
        double rForward = forwardResistance();
        voltage = vSource * (rForward / (rBack + rForward));
    }

    private double reverseResistance() {
        if (fixed) {
            return 0;
        } else {
            double temp = 0;
            for (Component c : reverse) {
                temp += 1 / (c.getResistance() + c.getLeft().reverseResistance());
            }
            return 1 / temp;
        }
    }

    private double forwardResistance() {
        if (fixed) {
            return 0;
        } else {
            double temp = 0;
            for (Component c : forward) {
                temp += 1 / (c.getResistance() + c.getRight().forwardResistance());
            }
            return 1 / temp;
        }
    }
}
