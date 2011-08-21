package elecsim1;

abstract class Component {
    private Node left;
    private Node right;
    
    
    Node getLeft() {
        return left;
    }

    void setLeft(Node left) {
        this.left = left;
    }

    Node getRight() {
        return right;
    }

    void setRight(Node right) {
        this.right = right;
    }

    void disconnect() {
        left = right = null;
    }
}
