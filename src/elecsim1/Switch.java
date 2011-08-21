
package elecsim1;

class Switch extends Component {
    private boolean closed;
    
    Switch(boolean closed) {
        this.closed = closed;
    }
    
    Switch() {
        
    }
    
    boolean isClosed() {
        return closed;
    }
}
