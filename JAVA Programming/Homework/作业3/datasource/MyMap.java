import java.util.Iterator;
public interface MyMap<K, V> {

 
    int	size();
    boolean	isEmpty();
    V get(K key);
    V put(K key, V value);
    V remove(K key);
    Iterator<Entry<K, V>> iterator();
}
