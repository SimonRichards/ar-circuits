package elecsim1;

import java.util.ArrayList;
import java.util.Collection;
import java.util.LinkedList;

class Node {
    
    private Collection<Component> components;
    private Collection<Component> otherComponents;

    public Node() {
        components = new LinkedList<Component>();
        otherComponents = new ArrayList<Component>(10);
    }    
    
    void removeComponent(Component component) {
        components.remove(component);
    }
    
    void addComponent(Component component) {
        components.add(component);
    }
    
    Collection<Component> allComponentsExcept(Component caller) {
        otherComponents.clear();
        for (Component c : components) {
            if (c != caller) 
                otherComponents.add(c);
        }
        return otherComponents;
    }    
}
