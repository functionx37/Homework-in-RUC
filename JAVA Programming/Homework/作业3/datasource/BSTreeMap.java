import java.util.Iterator;
import java.util.Stack;

public class BSTreeMap<K extends Comparable<K>, V> implements MyMap<K, V> {
    public static final int PREORDER = 1, INORDER = 2, POSTORDER = 3;
    protected Node<K, V> root;
    protected int size;

    public BSTreeMap() { }

   
    public BSTreeMap(Pair<K, V>[] elements) {
        insertElements(elements);
    }

 
    public BSTreeMap(Pair<K, V>[] elements, boolean sorted) {
        if (!sorted) {
            insertElements(elements);
        } else {
            root = createBST(elements, 0, elements.length - 1);
        }
    }

   
    protected Node<K, V> createBST(Pair<K, V>[] pairs, int low, int high) {
        if (low > high) {
            return null;
        }
        int mid = low + (high - low) / 2;
        Pair<K, V> pair = pairs[mid];
        Node<K, V> parent = new Node<>(pair.key, pair.value);
        size++;
        parent.setLeft(createBST(pairs, low, mid - 1));
        if (parent.getLeft() != null) {
            parent.getLeft().setParent(parent);
        }
        parent.setRight(createBST(pairs, mid + 1, high));
        if (parent.getRight() != null) {
            parent.getRight().setParent(parent);
        }
        return parent;
    }

   
    protected void insertElements(Pair<K, V>[] pairs) {
        for (Pair<K, V> pair : pairs) {
            put(pair);
        }
    }

   
    public int size() {
        return size;
    }

    
    public boolean isEmpty() {
        return size == 0;
    }

    
    public String preorder() {
        StringBuilder builder = new StringBuilder();
        builder.append("[");
        preorder(root, builder, 0);
        builder.append("]");
        return builder.toString();
    }

    private int preorder(Node<K, V> n, StringBuilder builder,
                         int nodesVisited) {
        if (n != null) {
            if (nodesVisited != 0) {
                builder.append(", ");
            }
            builder.append(n);
            nodesVisited = preorder(n.getLeft(), builder, nodesVisited + 1);
            nodesVisited = preorder(n.getRight(), builder, nodesVisited);
        }
        return nodesVisited;
    }

   
    public String inorder() {
        StringBuilder builder = new StringBuilder();
        builder.append("[");
        inorder(root, builder, 0);
        builder.append("]");
        return builder.toString();
    }


    private int inorder(Node<K, V> n, StringBuilder builder,
                        int nodesVisited) {
        if (n != null) {
            nodesVisited = inorder(n.getLeft(), builder, nodesVisited);
            if (nodesVisited != 0) {
                builder.append(", ");
            }
            builder.append(n);
            nodesVisited = inorder(n.getRight(), builder, nodesVisited + 1);
        }
        return nodesVisited;
    }

   
    public String postorder() {
        StringBuilder builder = new StringBuilder();
        builder.append("[");
        postorder(root, builder, 0);
        builder.append("]");
        return builder.toString();
    }

   
    private int postorder(Node<K, V> n, StringBuilder builder,
                          int nodesVisited) {
        if (n != null) {
            nodesVisited = postorder(n.getLeft(), builder, nodesVisited);
            nodesVisited = postorder(n.getRight(), builder, nodesVisited);
            if (nodesVisited != 0) {
                builder.append(", ");
            }
            builder.append(n);
            nodesVisited++;
        }
        return nodesVisited;
    }

  
    public V get(K key) {
        Node<K, V> x = iterativeSearch(key);
        return x != null ? x.value : null;
    }

  
    protected Node<K, V> iterativeSearch(K key) {
        Node<K, V> x = root;
        while (x != null) {
            int comparison = key.compareTo(x.key);
            if (comparison == 0) {
                break; // Found!
            } else if (comparison < 0) {
                x = x.getLeft();
            } else {
                x = x.getRight();
            }
        }
        return x;
    }

  
    public V put(Pair<K, V> pair) {
        return put(pair.key, pair.value);
    }

    
    public V put(K key, V value) {
        Node<K, V> x = root, y = null;
        while (x != null) {
            y = x;
            int comparison = key.compareTo(x.key);
            if (comparison < 0) {
                x = x.getLeft();
            } else if (comparison > 0) {
                x = x.getRight();
            } else {
                // The key was found in the tree. Return the previous value
                // associated with the key.
                V oldValue = x.value;
                x.value = value;
                return oldValue;
            }
        }
        Node<K, V> z = new Node<>(key, value);
        z.setParent(y);
        if (y == null) {
            root = z;
        } else if (key.compareTo(y.key) < 0) {
            y.setLeft(z);
        } else {
            y.setRight(z);
        }
        size++;
        // There was no previous mapping for this key.
        return null;
    }


    public V remove(K key) {
        Node<K, V> z = iterativeSearch(key);
        if (z == null) {
            return null;
        }
        if (z.getLeft() == null) {
            transplant(z, z.getRight());
        } else if (z.getRight() == null) {
            transplant(z, z.getLeft());
        } else {
            Node<K, V> y = treeMinimum(z.getRight());
            if (y.getParent() != z) {
                transplant(y, y.getRight());
                y.setRight(z.getRight());
                y.getRight().setParent(y);
            }
            transplant(z, y);
            y.setLeft(z.getLeft());
            y.getLeft().setParent(y);
        }
        size--;
        return z.value;
    }

  
    protected Node<K, V> treeMinimum(Node<K, V> x) {
        while (x.getLeft() != null) {
            x = x.getLeft();
        }
        return x;
    }

    protected void transplant(Node<K, V> u, Node<K, V> v) {
        if (u.getParent() == null) {
            root = v;
        } else if (u == u.getParent().getLeft()) {
            u.getParent().setLeft(v);
        } else {
            u.getParent().setRight(v);
        }
        if (v != null) {
            v.setParent(u.getParent());
        }
    }

  
    public String toString() {
        return inorder();
    }

   
    public String toAsciiDrawing() {
        BinarySearchTreePrinter<K, V> printer =
                new BinarySearchTreePrinter<>();
        printer.createAsciiTree(root);
        return printer.toString();
    }

    public void printTraversal(int type) {
        switch (type) {
            case PREORDER -> {
                System.out.print("Preorder traversal:       ");
                System.out.println(preorder());
            }
            case INORDER -> {
                System.out.print("Inorder traversal:        ");
                System.out.println(inorder());
            }
            case POSTORDER -> {
                System.out.print("Postorder traversal:      ");
                System.out.println(postorder());
            }
        }
    }

   
    public Iterator<Entry<K, V>> iterator() {
        return new BinaryTreeItr();
    }

    private class BinaryTreeItr implements Iterator<Entry<K, V>> {
        private Node<K, V> current;
        private final Stack<Node<K, V>> parentStack = new Stack<>();

        BinaryTreeItr() {
            current = root;
        }

        @Override
        public boolean hasNext() {
            return !parentStack.isEmpty() || current != null;
        }

        @Override
        public Entry<K, V> next() {
            while (hasNext()) {
                if (current != null) {
                    parentStack.push(current);
                    current = current.getLeft();
                } else {
                    Node<K, V> toReturn = parentStack.pop();
                    current = toReturn.getRight();
                    return toReturn;
                }
            }
            return null;
        }

        @Override
        public void remove() {
            throw new UnsupportedOperationException();
        }
    }

  
    public int height() {
        return height(root) ;
    }

    protected int height(Node<K, V> node) {
        if (node == null) {
            return -1;
        }
        return 1 + Math.max(height(node.getLeft()), height(node.getRight()));
    }

  
    public int nullCount() {
        return nullCount(root);
    }

    private int nullCount(Node<K, V> node) {
        if (node == null) {
            return 1;
        }
        return nullCount(node.getLeft()) + nullCount(node.getRight());
    }

    /**
     * Returns the sum of the levels of each non-null node in the tree starting
     * at the root.
     * For example, the tree
     *   5 <- level 0
     *  / \
     * 2   8 <- level 1
     *      \
     *       10 <- level 2
     * has sum 0 + 2(1) + 2 = 4.
     * @return the sum of the levels of each non-null node in the tree starting
     *         at the root
     */
    public int sumLevels() {
        return sumLevels(root, 0);
    }

    private int sumLevels(Node<K, V> node, int level) {
        if (node == null) {
            return 0;
        }
        return sumLevels(node.getLeft(), level + 1) +
               sumLevels(node.getRight(), level + 1) + level;
    }

    /**
     * Returns the sum of the levels of each null node in the tree starting at
     * the root.
     * For example, the tree
     *    5 <- level 0
     *   / \
     *  2   8 <- level 1
     * / \ / \
     * * * * * 10 <- level 2
     *        / \
     *        * * <- level 3
     * has sum 3(2) + 2(3) = 12.
     * @return the sum of the levels of each null node in the tree starting at
     *         the root
     */
    public int sumNullLevels() {
        return sumNullLevels(root, 0);
    }

    private int sumNullLevels(Node<K, V> node, int level) {
        if (node == null) {
            return level;
        }
        return sumNullLevels(node.getLeft(), level + 1) +
               sumNullLevels(node.getRight(), level + 1);
    }

    public double successfulSearchCost() {
        return size == 0 ? 0 : 1 + (double)sumLevels() / size;
    }

    public double unsuccessfulSearchCost() {
        return (double)sumNullLevels() / nullCount();
    }
}
