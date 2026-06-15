/**
 * 基于二叉搜索树扩展出的红黑树映射。
 * 红黑树通过“节点染色 + 旋转 + 局部重构”来保证树高维持在 O(log n)，
 * 因而插入、删除、查找都能保持稳定效率。
 */
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

    /**
     * 根据有序键值对构造一棵近似平衡的树，并在最底层附近染成红色，
     * 这样能直接得到一棵满足大部分红黑树性质的初始结构。
     */
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

    /**
     * 红黑树删除的主体思路与 BST 类似：
     * 1. 先按 BST 方式找到待删节点；
     * 2. 如果左右子树都存在，则转化为删除后继节点；
     * 3. 真正删掉黑节点时，可能破坏黑高，需要 deleteFixup 修复。
     */
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

    /**
     * 插入的新节点默认染成红色，避免直接增加整条路径的黑高。
     * 若父节点也是红色，就违反了“红节点不能连续”这一性质，
     * 因此需要根据叔叔节点颜色分情况进行变色或旋转。
     */
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

    /**
     * 删除修复的核心是处理“额外黑色”问题。
     * 这里按照 CLRS 的四种经典情形分类：
     * 兄弟为红、兄弟为黑且两个孩子都黑、兄弟为黑且近侄子为红、
     * 兄弟为黑且远侄子为红。通过旋转和重新染色把失衡逐步向上消除。
     */
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

    /**
     * 左旋：把 x 的右孩子提到上方，x 下沉为其左孩子。
     * 左旋不会破坏二叉搜索树的中序次序，只会局部调整树形。
     */
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

    /**
     * 右旋：和左旋完全对称，把 x 的左孩子提到上方，x 下沉为其右孩子。
     */
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
