import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.util.Arrays;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Iterator;
import java.util.List;
import java.util.Locale;
import java.util.Map;
import java.util.HashMap;
import java.util.TreeMap;

public class MostAnagramsFinder {
    private static final String USAGE_MESSAGE = "Usage: java MostAnagramsFinder";

    public static void main(String[] args) {
        if (args.length < 1 || args.length > 2) {
            System.err.println(USAGE_MESSAGE);
            return;
        }

        File inputFile = new File(args[0]);
        if (!inputFile.exists() || !inputFile.isFile()) {
            System.err.println("Error: Cannot open file '" + args[0] + "' for input.");
            return;
        }

        WordGroupStore store;
        try {
            store = createStore(args.length == 2 ? args[1] : "myhash");
        } catch (IllegalArgumentException exception) {
            System.err.println(USAGE_MESSAGE);
            return;
        }

        try {
            loadWords(inputFile, store);
        } catch (IOException exception) {
            System.err.println("Error: An I/O error occurred reading '" + args[0] + "'.");
            return;
        }

        GroupResult result = findLargestAnagramGroups(store);
        if (result.groups.isEmpty()) {
            System.out.println("No anagrams found.");
            return;
        }

        System.out.println("Groups: " + result.groupCount + ", Anagram count: " + result.anagramCount);
        for (List<String> group : result.groups) {
            System.out.println(formatGroup(group));
        }
    }

    private static WordGroupStore createStore(String type) {
        String normalizedType = type.toLowerCase(Locale.ROOT);
        switch (normalizedType) {
            case "myhash":
            case "myhashmap":
                return new CustomStore(new MyHashMap<String, List<String>>());
            case "bst":
            case "bstree":
            case "bstreemap":
                return new CustomStore(new BSTreeMap<String, List<String>>());
            case "rb":
            case "rbt":
            case "rbtree":
            case "rbtreemap":
                return new CustomStore(new RBTreeMap<String, List<String>>());
            case "hash":
            case "hashmap":
            case "javahash":
            case "javahashmap":
                return new JavaStore(new HashMap<String, List<String>>());
            case "tree":
            case "treemap":
            case "javatree":
            case "javatreemap":
                return new JavaStore(new TreeMap<String, List<String>>());
            default:
                throw new IllegalArgumentException("Unsupported map type: " + type);
        }
    }

    private static void loadWords(File inputFile, WordGroupStore store) throws IOException {
        try (BufferedReader reader = new BufferedReader(new FileReader(inputFile))) {
            String word;
            while ((word = reader.readLine()) != null) {
                word = word.trim();
                if (word.isEmpty()) {
                    continue;
                }

                String signature = buildSignature(word);
                List<String> group = store.get(signature);
                if (group == null) {
                    group = new ArrayList<>();
                    store.put(signature, group);
                }
                group.add(word);
            }
        }
    }

    private static String buildSignature(String word) {
        char[] letters = word.toLowerCase(Locale.ROOT).toCharArray();
        Arrays.sort(letters);
        return new String(letters);
    }

    private static GroupResult findLargestAnagramGroups(WordGroupStore store) {
        int maxCount = 0;
        List<List<String>> groups = new ArrayList<>();

        for (StoreEntry entry : store.entries()) {
            List<String> words = entry.value;
            if (words.size() < 2) {
                continue;
            }

            List<String> sortedGroup = new ArrayList<>(words);
            Collections.sort(sortedGroup);

            if (sortedGroup.size() > maxCount) {
                maxCount = sortedGroup.size();
                groups.clear();
                groups.add(sortedGroup);
            } else if (sortedGroup.size() == maxCount) {
                groups.add(sortedGroup);
            }
        }

        groups.sort((left, right) -> left.get(0).compareTo(right.get(0)));
        return new GroupResult(groups, maxCount);
    }

    private static String formatGroup(List<String> group) {
        return "[" + String.join(", ", group) + "]";
    }

    private interface WordGroupStore {
        List<String> get(String key);
        void put(String key, List<String> value);
        Iterable<StoreEntry> entries();
    }

    private static final class StoreEntry {
        private final List<String> value;

        private StoreEntry(List<String> value) {
            this.value = value;
        }
    }

    private static final class GroupResult {
        private final List<List<String>> groups;
        private final int anagramCount;
        private final int groupCount;

        private GroupResult(List<List<String>> groups, int anagramCount) {
            this.groups = groups;
            this.anagramCount = anagramCount;
            this.groupCount = groups.size();
        }
    }

    private static final class CustomStore implements WordGroupStore {
        private final MyMap<String, List<String>> map;

        private CustomStore(MyMap<String, List<String>> map) {
            this.map = map;
        }

        @Override
        public List<String> get(String key) {
            return map.get(key);
        }

        @Override
        public void put(String key, List<String> value) {
            map.put(key, value);
        }

        @Override
        public Iterable<StoreEntry> entries() {
            return () -> new Iterator<StoreEntry>() {
                private final Iterator<Entry<String, List<String>>> iterator = map.iterator();

                @Override
                public boolean hasNext() {
                    return iterator.hasNext();
                }

                @Override
                public StoreEntry next() {
                    Entry<String, List<String>> entry = iterator.next();
                    return new StoreEntry(entry.value);
                }
            };
        }
    }

    private static final class JavaStore implements WordGroupStore {
        private final Map<String, List<String>> map;

        private JavaStore(Map<String, List<String>> map) {
            this.map = map;
        }

        @Override
        public List<String> get(String key) {
            return map.get(key);
        }

        @Override
        public void put(String key, List<String> value) {
            map.put(key, value);
        }

        @Override
        public Iterable<StoreEntry> entries() {
            return () -> new Iterator<StoreEntry>() {
                private final Iterator<Map.Entry<String, List<String>>> iterator =
                        map.entrySet().iterator();

                @Override
                public boolean hasNext() {
                    return iterator.hasNext();
                }

                @Override
                public StoreEntry next() {
                    return new StoreEntry(iterator.next().getValue());
                }
            };
        }
    }
}
