package elecsim1;

import java.util.ArrayList;
import java.util.Collection;

class Simulation {
    private static final int NUM_NODES = 20;
    Collection<Node> nodes;
    Bulb[] bulbs;
    Switch[] switches;
    Component[] components;
    final Node sink;
    final Node source;

    public Simulation(double voltage, int numBulbs, int numSwitches) {
        source = new Node(voltage);
        sink = new Node(0);
        
        nodes = new ArrayList<>(NUM_NODES);
        bulbs = new Bulb[numBulbs];
        switches = new Switch[numSwitches];
        components = new Component[numSwitches + numBulbs];
        
        initArray(bulbs, Bulb.class, numBulbs, 0);
        initArray(switches, Switch.class, numSwitches, numBulbs);
    }
    
    private <T extends Component> void initArray(T[] array, Class<T> type, int num, int count) {
        for (int i = 0; i < num; i++) {
            T temp;
            try {
                temp = type.newInstance();
            } catch (ReflectiveOperationException e) {
                throw new RuntimeException("Components need no-arg constructors");
            }      
            array[i] = temp;
            components[i + count] = temp; 
        }
    }
    
    public void run() {
        for (Node node :nodes) {
//            node.
        }
    }
    
    Node newNode() {
        Node temp = new Node();
        nodes.add(temp);
        return temp;
    }
        
    Bulb getBulb(int id) {
        return bulbs[id];
    }
    
    Switch getSwitch(int id) {
        return switches[id];
    }
    
    public double getBulbPower(int id) {
        return bulbs[id].getPower();
    }
        
    public void scrub() {
        source.scrub();
        sink.scrub();
        nodes.clear();
        for (Component c : components) 
            c.disconnect();
    }
            
}
