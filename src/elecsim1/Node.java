package elecsim1;

import java.util.Collection;
import java.util.LinkedList;

class Node {
    
    private Collection<Component> reverse;
    private Collection<Component> forward;
    double voltage;
    boolean fixed;
    
    Node() {
        this(0);
        fixed = false;
    }

    Node(double voltage) {
        this.voltage = voltage;
        reverse = new LinkedList<Component>();
        forward = new LinkedList<Component>();
        fixed = true;
    }    
    
    
    void addBehind(Component component) {
        reverse.remove(component);
    }
    
    void addFront(Component component) {
        forward.add(component);
    }
      
    void merge(Node other) {
        reverse.addAll(other.reverse);
        forward.addAll(other.forward);
    }

    void scrub() {
        reverse.clear();
        forward.clear();
    }
}
