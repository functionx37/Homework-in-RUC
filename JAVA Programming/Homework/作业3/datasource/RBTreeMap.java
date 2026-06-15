public class RBTreeMap<K extends Comparable<K>, V> extends BSTreeMap<K, V>
        implements MyMap<K, V> {

   
    public RBTreeMap() { }

      public RBTreeMap(Pair<K, V>[] elements) {
        insertElements(elements);
    }


    public RBTreeMap(Pair<K, V>[] elements, boolean sorted) {
        if (!sorted) {
            insertElements(elements);
        } else {
            root = createBST(elements, 0, elements.length - 1);
        }
    }

    protected Node<K, V> createBST(Pair<K, V>[] pairs, int low, int high) {
        return createBSTHelper(
                pairs, low, high, null, 0,
                (int)(Math.log(pairs.length + 1) / Math.log(2)));
    }

    private Node<K, V> createBSTHelper(
            Pair<K, V>[] pairs, int low, int high, Node<K, V> parent, int level,
            int redLevel) {
        if (low > high) {
            return null;
        }
        int mid = low + (high - low) / 2;
        Pair<K, V> pair = pairs[mid];
        RBNode<K, V> node = new RBNode<>(pair.key, pair.value);
        node.color = (level == redLevel) ? RBNode.RED : RBNode.BLACK;
        node.setParent(parent);
        node.setLeft(
            createBSTHelper(
                    pairs, low, mid - 1, node, level + 1, redLevel));
        node.setRight(
            createBSTHelper(
                    pairs, mid + 1, high, node, level + 1, redLevel));
        size++;
        return node;
    }

     @Override
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
                // associated with the key. There's no reason to balance.
                V oldValue = x.value;
                x.value = value;
                return oldValue;
            }
        }
        RBNode<K, V> z = new RBNode<>(key, value);
        z.setParent(y);
        if (y == null) {
            root = z;
        } else if (key.compareTo(y.key) < 0) {
            y.setLeft(z);
        } else {
            y.setRight(z);
        }
        insertFixup(z);
        size++;
        return null;
    }

      public V remove(K key) {
        RBNode<K, V> z = (RBNode<K, V>)iterativeSearch(key);
        if (z == null) {
            return null;
        }
        // Keep a reference to the original value for the method's return value.
        V originalValue = z.value;

        // If z has two children, swap key-value pair with the leftmost
        // node in the right subtree of z.
        if (z.getLeft() != null && z.getRight() != null) {
            RBNode<K, V> y = (RBNode<K, V>)treeMinimum(z.getRight());
            z.key = y.key;
            z.value = y.value;
            z = y;
        }

        RBNode<K, V> replacementNode = (z.getLeft() != null ? z.getLeft() :
                                                              z.getRight());
        if (replacementNode != null) {
            replacementNode.setParent(z.getParent());
            if (z.getParent() == null) {
                root = replacementNode;
            } else if (z == z.getParent().getLeft()) {
                z.getParent().setLeft(replacementNode);
            } else {
                z.getParent().setRight(replacementNode);
            }

            // Unlink node z from tree.
            z.setLeft(null);
            z.setRight(null);
            z.setParent(null);

            if (z.color == RBNode.BLACK) {
                deleteFixup(replacementNode);
            }
        } else if (z.getParent() == null) {
            root = null;
        } else {
            if (z.color == RBNode.BLACK) {
                deleteFixup(z);
            }
            if (z.getParent() != null) {
                if (z == z.getParent().getLeft()) {
                    z.getParent().setLeft(null);
                } else if (z == z.getParent().getRight()) {
                    z.getParent().setRight(null);
                }
                z.setParent(null);
            }
        }

        size--;
        return originalValue;
    }

    private void insertFixup(RBNode<K, V> z) {
        while (z != root && z.getParent().color == RBNode.RED) {
            if (z.getParent() == z.getParent().getParent().getLeft()) {
                RBNode<K, V> y = z.getParent().getParent().getRight();
                if (y != null && y.color == RBNode.RED) {
                    z.getParent().color = RBNode.BLACK;
                    y.color = RBNode.BLACK;
                    z.getParent().getParent().color = RBNode.RED;
                    z = z.getParent().getParent();
                } else {
                    if (z == z.getParent().getRight()) {
                        z = z.getParent();
                        leftRotate(z);
                    }
                    z.getParent().color = RBNode.BLACK;
                    z.getParent().getParent().color = RBNode.RED;
                    rightRotate(z.getParent().getParent());
                }
            } else {
                RBNode<K, V> y = z.getParent().getParent().getLeft();
                if (y != null && y.color == RBNode.RED) {
                    z.getParent().color = RBNode.BLACK;
                    y.color = RBNode.BLACK;
                    z.getParent().getParent().color = RBNode.RED;
                    z = z.getParent().getParent();
                } else {
                    if (z == z.getParent().getLeft()) {
                        z = z.getParent();
                        rightRotate(z);
                    }
                    z.getParent().color = RBNode.BLACK;
                    z.getParent().getParent().color = RBNode.RED;
                    leftRotate(z.getParent().getParent());
                }
            }
        }
        ((RBNode<K, V>)root).color = RBNode.BLACK;
    }

    // Fixup method
    
    private void deleteFixup(RBNode<K, V> x) {
        while (x != root && colorOf(x) == RBNode.BLACK) {
            if (x == leftOf(parentOf(x))) {
                RBNode<K, V> w = rightOf(parentOf(x));
                if (colorOf(w) == RBNode.RED) {
                    setColor(w, RBNode.BLACK);
                    setColor(parentOf(x), RBNode.RED);
                    leftRotate(parentOf(x));
                    w = rightOf(parentOf(x));
                }
                if (colorOf(leftOf(w))  == RBNode.BLACK &&
                        colorOf(rightOf(w)) == RBNode.BLACK) {
                    setColor(w,  RBNode.RED);
                    x = parentOf(x);
                } else {
                    if (colorOf(rightOf(w)) == RBNode.BLACK) {
                        setColor(leftOf(w), RBNode.BLACK);
                        setColor(w, RBNode.RED);
                        rightRotate(w);
                        w = rightOf(parentOf(x));
                    }
                    setColor(w, colorOf(parentOf(x)));
                    setColor(parentOf(x), RBNode.BLACK);
                    setColor(rightOf(w), RBNode.BLACK);
                    leftRotate(parentOf(x));
                    x = (RBNode<K, V>)root;
                }
            } else {
                RBNode<K ,V> w = leftOf(parentOf(x));
                if (colorOf(w) == RBNode.RED) {
                    setColor(w, RBNode.BLACK);
                    setColor(parentOf(x), RBNode.RED);
                    rightRotate(parentOf(x));
                    w = leftOf(parentOf(x));
                }
                if (colorOf(rightOf(w)) == RBNode.BLACK &&
                        colorOf(leftOf(w)) == RBNode.BLACK) {
                    setColor(w,  RBNode.RED);
                    x = parentOf(x);
                } else {
                    if (colorOf(leftOf(w)) == RBNode.BLACK) {
                        setColor(rightOf(w), RBNode.BLACK);
                        setColor(w, RBNode.RED);
                        leftRotate(w);
                        w = leftOf(parentOf(x));
                    }
                    setColor(w, colorOf(parentOf(x)));
                    setColor(parentOf(x), RBNode.BLACK);
                    setColor(leftOf(w), RBNode.BLACK);
                    rightRotate(parentOf(x));
                    x = (RBNode<K, V>)root;
                }
            }
        }
        setColor(x, RBNode.BLACK);
    }

    private byte colorOf(RBNode<K, V> node) {
        return node == null ? RBNode.BLACK : node.color;
    }

    private RBNode<K,V> parentOf(RBNode<K, V> node) {
        return node == null ? null: node.getParent();
    }

    private void setColor(RBNode<K, V> node, byte color) {
        if (node != null) {
            node.color = color;
        }
    }

    private RBNode<K, V> leftOf(RBNode<K, V> node) {
        return node == null ? null: node.getLeft();
    }

    private RBNode<K, V> rightOf(RBNode<K, V> node) {
        return node == null ? null: node.getRight();
    }

    // Left-rotate method 
   
    private void leftRotate(Node<K, V> x) {
        Node<K, V> y = x.getRight();
        x.setRight(y.getLeft());
        if (y.getLeft() != null) {
            y.getLeft().setParent(x);
        }
        y.setParent(x.getParent());
        if (x.getParent() == null) {
            root = y;
        } else if (x == x.getParent().getLeft()) {
            x.getParent().setLeft(y);
        } else {
            x.getParent().setRight(y);
        }
        y.setLeft(x);
        x.setParent(y);
    }

    //Right-rotate method described on p. 336 of CLRS, 4e.
     
    private void rightRotate(Node<K, V> x) {
        Node<K, V> y = x.getLeft();
        x.setLeft(y.getRight());
        if (y.getRight() != null) {
            y.getRight().setParent(x);
        }
        y.setParent(x.getParent());
        if (x.getParent() == null) {
            root = y;
        } else if (x == x.getParent().getRight()) {
            x.getParent().setRight(y);
        } else {
            x.getParent().setLeft(y);
        }
        y.setRight(x);
        x.setParent(y);
    }
}