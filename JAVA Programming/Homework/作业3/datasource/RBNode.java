public class RBNode<K, V> extends Node<K, V> {
    public final static byte BLACK = 0, RED = 1;
    public final static String[] COLOR = {"B", "R"};

    byte color = RED;  // Used only in RBTreeMap, defaults to red.
    public RBNode(K key, V value) {
        super(key, value);
    }

    public final RBNode<K, V> getLeft() {
        return (RBNode<K, V>)super.getLeft();
    }

    public final RBNode<K, V> getRight() {
        return (RBNode<K, V>)super.getRight();
    }

    public final RBNode<K, V> getParent() {
        return (RBNode<K, V>)super.getParent();
    }

    public String toString() {
        return "<" + key + ", " + value + ", " + COLOR[color] + ">";
    }
}
