public class Node<K, V> extends Entry<K, V> {
    private Node<K, V> left, right, parent;
    public Node(K key, V value) {
        super(key, value);
    }
    public Node<K, V> getLeft() {
        return this.left;
    }

    public Node<K, V> getRight() {
        return this.right;
    }

    public Node<K, V> getParent() {
        return this.parent;
    }
    public void setLeft(Node<K, V> node) {
        this.left = node;
    }

    public void setRight(Node<K, V> node) {
        this.right = node;
    }

    public void setParent(Node<K, V> node) {
        this.parent = node;
    }
    
    public String toString() {
        return "<" + key + ", " + value + ">";
    }
}
