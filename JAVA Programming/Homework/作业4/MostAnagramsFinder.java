import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Locale;
import java.util.Map;
import java.util.TreeMap;

public class MostAnagramsFinder {
    private static final String USAGE_MESSAGE = "Usage: java MostAnagramsFinder";

    public static void main(String[] args) {
        ProgramOptions options;
        try {
            options = ProgramOptions.parse(args);
        } catch (IllegalArgumentException exception) {
            System.err.println(USAGE_MESSAGE);
            return;
        }

        File inputFile = new File(options.inputFilePath);
        if (!inputFile.exists() || !inputFile.isFile()) {
            System.err.println("Error: Cannot open file '" + args[0] + "' for input.");
            return;
        }

        long startTime = System.nanoTime();
        AnalysisResult result;
        try {
            AnagramFinder finder = new AnagramFinder(options.mapType.createStore());
            result = finder.analyze(inputFile);
        } catch (IOException exception) {
            System.err.println("Error: An I/O error occurred reading '" + args[0] + "'.");
            return;
        }

        if (result.groups.isEmpty()) {
            System.out.println("No anagrams found.");
        } else {
            System.out.println("Groups: " + result.groupCount + ", Anagram count: " + result.anagramCount);
            for (List<String> group : result.groups) {
                System.out.println(formatGroup(group));
            }
        }

        if (options.benchmarkEnabled) {
            double elapsedMilliseconds = (System.nanoTime() - startTime) / 1_000_000.0;
            System.err.printf(Locale.ROOT, "MapType=%s, Time=%.3f ms%n",
                    options.mapType.getDisplayName(), elapsedMilliseconds);
        }
    }

    private static String formatGroup(List<String> group) {
        return "[" + String.join(", ", group) + "]";
    }

    private static final class AnagramFinder {
        private final WordGroupStore store;

        private AnagramFinder(WordGroupStore store) {
            this.store = store;
        }

        private AnalysisResult analyze(File inputFile) throws IOException {
            loadWords(inputFile);
            return buildResult();
        }

        private void loadWords(File inputFile) throws IOException {
            try (BufferedReader reader = new BufferedReader(
                    new InputStreamReader(new FileInputStream(inputFile), StandardCharsets.UTF_8), 64 * 1024)) {
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

        private AnalysisResult buildResult() {
            int maxCount = 0;
            List<List<String>> winnerGroups = new ArrayList<>();

            for (StoreEntry entry : store.entries()) {
                int currentSize = entry.value.size();
                if (currentSize < 2) {
                    continue;
                }

                if (currentSize > maxCount) {
                    maxCount = currentSize;
                    winnerGroups.clear();
                    winnerGroups.add(entry.value);
                } else if (currentSize == maxCount) {
                    winnerGroups.add(entry.value);
                }
            }

            if (maxCount < 2) {
                return new AnalysisResult(Collections.emptyList(), 0);
            }

            List<List<String>> sortedGroups = new ArrayList<>();
            for (List<String> group : winnerGroups) {
                List<String> sortedGroup = new ArrayList<>(group);
                Collections.sort(sortedGroup);
                sortedGroups.add(sortedGroup);
            }

            sortedGroups.sort((left, right) -> left.get(0).compareTo(right.get(0)));
            return new AnalysisResult(sortedGroups, maxCount);
        }

        private String buildSignature(String word) {
            char[] letters = word.toLowerCase(Locale.ROOT).toCharArray();
            Arrays.sort(letters);
            return new String(letters);
        }
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

    private static final class AnalysisResult {
        private final List<List<String>> groups;
        private final int anagramCount;
        private final int groupCount;

        private AnalysisResult(List<List<String>> groups, int anagramCount) {
            this.groups = groups;
            this.anagramCount = anagramCount;
            this.groupCount = groups.size();
        }
    }

    private static final class ProgramOptions {
        private final String inputFilePath;
        private final MapType mapType;
        private final boolean benchmarkEnabled;

        private ProgramOptions(String inputFilePath, MapType mapType, boolean benchmarkEnabled) {
            this.inputFilePath = inputFilePath;
            this.mapType = mapType;
            this.benchmarkEnabled = benchmarkEnabled;
        }

        private static ProgramOptions parse(String[] args) {
            if (args.length < 1 || args.length > 3) {
                throw new IllegalArgumentException("参数数量不合法");
            }

            String inputFilePath = args[0];
            MapType mapType = MapType.MY_HASH_MAP;
            boolean benchmarkEnabled = false;
            boolean explicitMapTypeProvided = false;

            for (int i = 1; i < args.length; i++) {
                String currentArg = args[i];
                if ("--benchmark".equalsIgnoreCase(currentArg)) {
                    benchmarkEnabled = true;
                    continue;
                }

                if (explicitMapTypeProvided) {
                    throw new IllegalArgumentException("重复指定数据结构");
                }

                mapType = MapType.fromAlias(currentArg);
                explicitMapTypeProvided = true;
            }

            return new ProgramOptions(inputFilePath, mapType, benchmarkEnabled);
        }
    }

    private enum MapType {
        MY_HASH_MAP("MyHashMap", new String[]{"myhash", "myhashmap"}) {
            @Override
            WordGroupStore createStore() {
                return new CustomStore(new MyHashMap<String, List<String>>());
            }
        },
        BST_TREE_MAP("BSTreeMap", new String[]{"bst", "bstree", "bstreemap"}) {
            @Override
            WordGroupStore createStore() {
                return new CustomStore(new BSTreeMap<String, List<String>>());
            }
        },
        RB_TREE_MAP("RBTreeMap", new String[]{"rb", "rbt", "rbtree", "rbtreemap"}) {
            @Override
            WordGroupStore createStore() {
                return new CustomStore(new RBTreeMap<String, List<String>>());
            }
        },
        JAVA_HASH_MAP("HashMap", new String[]{"hash", "hashmap", "javahash", "javahashmap"}) {
            @Override
            WordGroupStore createStore() {
                return new JavaStore(new HashMap<String, List<String>>());
            }
        },
        JAVA_TREE_MAP("TreeMap", new String[]{"tree", "treemap", "javatree", "javatreemap"}) {
            @Override
            WordGroupStore createStore() {
                return new JavaStore(new TreeMap<String, List<String>>());
            }
        };

        private final String displayName;
        private final String[] aliases;

        MapType(String displayName, String[] aliases) {
            this.displayName = displayName;
            this.aliases = aliases;
        }

        abstract WordGroupStore createStore();

        private String getDisplayName() {
            return displayName;
        }

        private static MapType fromAlias(String alias) {
            String normalizedAlias = alias.toLowerCase(Locale.ROOT);
            for (MapType mapType : values()) {
                for (String candidate : mapType.aliases) {
                    if (candidate.equals(normalizedAlias)) {
                        return mapType;
                    }
                }
            }
            throw new IllegalArgumentException("不支持的数据结构类型");
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
                private final Iterator<Map.Entry<String, List<String>>> iterator = map.entrySet().iterator();

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
