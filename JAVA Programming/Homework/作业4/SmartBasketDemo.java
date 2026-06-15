import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.time.LocalDate;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Comparator;
import java.util.EnumMap;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class SmartBasketDemo {
    public static void main(String[] args) {
        GenericBasket<Product> basket = new GenericBasket<>(10);

        try {
            DemoProductFactory demoFactory = DemoProductFactory.defaultFactory();

            System.out.println("=== 1. 用工厂模式创建测试商品并加入购物篮 ===");
            for (Product product : demoFactory.createDemoProducts()) {
                basket.addProduct(product);
            }
            basket.printDetails();

            System.out.println("\n=== 2. 演示异常捕获 ===");
            try {
                basket.removeProduct(20);
            } catch (RuntimeException e) {
                System.out.println("捕获到异常：" + e.getMessage());
            }

            System.out.println("\n=== 3. 按价格升序排序 ===");
            basket.sortByPrice(true);
            basket.printSimpleList();

            System.out.println("\n=== 4. 查找价格低于 100 的商品 ===");
            List<Product> affordableProducts = basket.findProductsBelowPrice(100);
            for (Product product : affordableProducts) {
                System.out.println(product.toDisplayString());
            }

            System.out.println("\n=== 5. 促销与价格计算策略 ===");
            System.out.printf("原始总价：%.2f%n", basket.getTotalPrice());

            DiscountStrategy percentageDiscount = new PercentageDiscountStrategy("全场9折", 0.9);
            DiscountStrategy fullReduction = new FullReductionDiscountStrategy("满100减10", 100, 10);
            DiscountStrategy stackedDiscount = new CompositeDiscountStrategy(
                    "先9折再满100减10",
                    Arrays.asList(percentageDiscount, fullReduction));

            printDiscountResult(basket, percentageDiscount);
            printDiscountResult(basket, fullReduction);
            printDiscountResult(basket, stackedDiscount);

            DiscountPlan bestPlan = PromotionPlanner.findBestPlan(
                    basket.getTotalPrice(),
                    Arrays.asList(percentageDiscount, fullReduction, stackedDiscount));
            System.out.printf("最优方案：%s，折后总价：%.2f，优惠金额：%.2f%n",
                    bestPlan.getStrategyName(), bestPlan.getDiscountedTotal(), bestPlan.getSavedAmount());

            System.out.println("\n=== 6. 保存购物篮到文件 ===");
            Path savePath = Paths.get("basket-data-lesson4.json");
            BasketPersistence.saveBasket(basket, savePath);
            System.out.println("已保存到：" + savePath.toAbsolutePath());

            System.out.println("\n=== 7. 从文件加载购物篮并打印 ===");
            GenericBasket<Product> loadedBasket = BasketPersistence.loadBasket(savePath, 10);
            loadedBasket.printDetails();
        } catch (Exception e) {
            System.out.println("程序执行失败：" + e.getMessage());
        }
    }

    private static void printDiscountResult(GenericBasket<Product> basket, DiscountStrategy strategy) {
        double discountedTotal = basket.getDiscountedTotalPrice(strategy);
        double savedAmount = basket.getTotalPrice() - discountedTotal;
        System.out.printf("%s -> 折后总价：%.2f，优惠金额：%.2f%n",
                strategy.getName(), discountedTotal, savedAmount);
    }
}

interface Product {
    String getName();

    String getCategory();

    double getPrice();

    String getDescription();

    String getExtraLabel();

    String getExtraValue();

    String toDisplayString();
}

abstract class AbstractProduct implements Product {
    private final String name;
    private final double price;
    private final String description;

    protected AbstractProduct(String name, double price, String description) {
        if (name == null || name.trim().isEmpty()) {
            throw new IllegalArgumentException("商品名称不能为空");
        }
        if (price <= 0) {
            throw new IllegalArgumentException("商品价格必须大于 0");
        }
        if (description == null || description.trim().isEmpty()) {
            throw new IllegalArgumentException("商品描述不能为空");
        }
        this.name = name.trim();
        this.price = price;
        this.description = description.trim();
    }

    @Override
    public String getName() {
        return name;
    }

    @Override
    public double getPrice() {
        return price;
    }

    @Override
    public String getDescription() {
        return description;
    }

    @Override
    public String toDisplayString() {
        return String.format("类型=%s, 名称=%s, 单价=%.2f, 描述=%s, %s=%s",
                getCategory(), getName(), getPrice(), getDescription(), getExtraLabel(), getExtraValue());
    }
}

class Food extends AbstractProduct {
    private final LocalDate expiryDate;

    public Food(String name, double price, String description, LocalDate expiryDate) {
        super(name, price, description);
        if (expiryDate == null) {
            throw new IllegalArgumentException("食品保质期不能为空");
        }
        this.expiryDate = expiryDate;
    }

    @Override
    public String getCategory() {
        return "食品";
    }

    @Override
    public String getExtraLabel() {
        return "保质期";
    }

    @Override
    public String getExtraValue() {
        return expiryDate.toString();
    }
}

class Book extends AbstractProduct {
    private final String author;

    public Book(String name, double price, String description, String author) {
        super(name, price, description);
        if (author == null || author.trim().isEmpty()) {
            throw new IllegalArgumentException("作者不能为空");
        }
        this.author = author.trim();
    }

    @Override
    public String getCategory() {
        return "书籍";
    }

    @Override
    public String getExtraLabel() {
        return "作者";
    }

    @Override
    public String getExtraValue() {
        return author;
    }
}

class Electronics extends AbstractProduct {
    private final String brand;

    public Electronics(String name, double price, String description, String brand) {
        super(name, price, description);
        if (brand == null || brand.trim().isEmpty()) {
            throw new IllegalArgumentException("品牌不能为空");
        }
        this.brand = brand.trim();
    }

    @Override
    public String getCategory() {
        return "电子产品";
    }

    @Override
    public String getExtraLabel() {
        return "品牌";
    }

    @Override
    public String getExtraValue() {
        return brand;
    }
}

class GenericBasket<T extends Product> {
    private final List<T> products;
    private final int capacity;

    public GenericBasket(int capacity) {
        if (capacity <= 0) {
            throw new IllegalArgumentException("购物篮容量必须大于 0");
        }
        this.capacity = capacity;
        this.products = new ArrayList<>();
    }

    public void addProduct(T product) {
        if (product == null) {
            throw new IllegalArgumentException("不能添加空商品");
        }
        if (products.size() >= capacity) {
            throw new BasketFullException("购物篮已满，无法继续添加商品");
        }
        products.add(product);
    }

    public T removeProduct(int index) {
        if (products.isEmpty()) {
            throw new BasketEmptyException("购物篮为空，无法删除商品");
        }
        if (index < 0 || index >= products.size()) {
            throw new IllegalArgumentException("删除失败，索引越界：" + index);
        }
        return products.remove(index);
    }

    public void sortByPrice(boolean ascending) {
        Comparator<T> comparator = Comparator.comparingDouble(Product::getPrice);
        if (!ascending) {
            comparator = comparator.reversed();
        }
        products.sort(comparator);
    }

    public List<T> findProductsBelowPrice(double priceLimit) {
        if (priceLimit < 0) {
            throw new IllegalArgumentException("价格上限不能小于 0");
        }
        List<T> matchedProducts = new ArrayList<>();
        for (T product : products) {
            if (product.getPrice() < priceLimit) {
                matchedProducts.add(product);
            }
        }
        return matchedProducts;
    }

    public double getTotalPrice() {
        double totalPrice = 0;
        for (T product : products) {
            totalPrice += product.getPrice();
        }
        return totalPrice;
    }

    public double getDiscountedTotalPrice(DiscountStrategy strategy) {
        if (strategy == null) {
            throw new IllegalArgumentException("折扣策略不能为空");
        }
        return strategy.applyDiscount(getTotalPrice());
    }

    public void printSimpleList() {
        if (products.isEmpty()) {
            System.out.println("购物篮为空");
            return;
        }
        for (int i = 0; i < products.size(); i++) {
            System.out.println((i + 1) + ". " + products.get(i).toDisplayString());
        }
    }

    public void printDetails() {
        System.out.println("当前商品总数：" + products.size() + "/" + capacity);
        printSimpleList();

        Map<String, Integer> categoryStats = buildCategoryStats();
        System.out.println("分类统计：");
        for (Map.Entry<String, Integer> entry : categoryStats.entrySet()) {
            System.out.println(entry.getKey() + " -> " + entry.getValue() + " 件");
        }

        T mostExpensiveProduct = findMostExpensiveProduct();
        if (mostExpensiveProduct != null) {
            System.out.println("最贵商品：" + mostExpensiveProduct.toDisplayString());
        }
    }

    public T findMostExpensiveProduct() {
        if (products.isEmpty()) {
            return null;
        }
        T mostExpensive = products.get(0);
        for (T product : products) {
            if (product.getPrice() > mostExpensive.getPrice()) {
                mostExpensive = product;
            }
        }
        return mostExpensive;
    }

    public List<T> getProductsSnapshot() {
        return new ArrayList<>(products);
    }

    public int getCapacity() {
        return capacity;
    }

    private Map<String, Integer> buildCategoryStats() {
        Map<String, Integer> categoryStats = new LinkedHashMap<>();
        for (T product : products) {
            categoryStats.put(product.getCategory(), categoryStats.getOrDefault(product.getCategory(), 0) + 1);
        }
        return categoryStats;
    }
}

enum ProductType {
    FOOD,
    BOOK,
    ELECTRONICS
}

final class ProductSpec {
    private final ProductType type;
    private final String name;
    private final double price;
    private final String description;
    private final String extraValue;

    public ProductSpec(ProductType type, String name, double price, String description, String extraValue) {
        this.type = type;
        this.name = name;
        this.price = price;
        this.description = description;
        this.extraValue = extraValue;
    }

    public ProductType getType() {
        return type;
    }

    public String getName() {
        return name;
    }

    public double getPrice() {
        return price;
    }

    public String getDescription() {
        return description;
    }

    public String getExtraValue() {
        return extraValue;
    }
}

interface ProductFactory {
    ProductType supports();

    Product createProduct(ProductSpec spec);
}

class FoodProductFactory implements ProductFactory {
    @Override
    public ProductType supports() {
        return ProductType.FOOD;
    }

    @Override
    public Product createProduct(ProductSpec spec) {
        return new Food(spec.getName(), spec.getPrice(), spec.getDescription(),
                LocalDate.parse(spec.getExtraValue()));
    }
}

class BookProductFactory implements ProductFactory {
    @Override
    public ProductType supports() {
        return ProductType.BOOK;
    }

    @Override
    public Product createProduct(ProductSpec spec) {
        return new Book(spec.getName(), spec.getPrice(), spec.getDescription(), spec.getExtraValue());
    }
}

class ElectronicsProductFactory implements ProductFactory {
    @Override
    public ProductType supports() {
        return ProductType.ELECTRONICS;
    }

    @Override
    public Product createProduct(ProductSpec spec) {
        return new Electronics(spec.getName(), spec.getPrice(), spec.getDescription(), spec.getExtraValue());
    }
}

class DemoProductFactory {
    private final Map<ProductType, ProductFactory> factories;

    private DemoProductFactory() {
        this.factories = new EnumMap<>(ProductType.class);
    }

    public static DemoProductFactory defaultFactory() {
        DemoProductFactory factory = new DemoProductFactory();
        factory.registerFactory(new FoodProductFactory());
        factory.registerFactory(new BookProductFactory());
        factory.registerFactory(new ElectronicsProductFactory());
        return factory;
    }

    public void registerFactory(ProductFactory factory) {
        factories.put(factory.supports(), factory);
    }

    public List<Product> createDemoProducts() {
        List<ProductSpec> specs = new ArrayList<>();
        specs.add(new ProductSpec(ProductType.FOOD, "纯牛奶", 12.5, "高钙牛奶", "2026-04-10"));
        specs.add(new ProductSpec(ProductType.FOOD, "苹果", 6.8, "新鲜红富士", "2026-03-30"));
        specs.add(new ProductSpec(ProductType.BOOK, "Java核心技术", 88.0, "适合进阶学习", "Cay S. Horstmann"));
        specs.add(new ProductSpec(ProductType.BOOK, "三体", 56.0, "科幻小说", "刘慈欣"));
        specs.add(new ProductSpec(ProductType.ELECTRONICS, "蓝牙耳机", 299.0, "主动降噪耳机", "SoundMax"));
        specs.add(new ProductSpec(ProductType.ELECTRONICS, "轻薄笔记本", 5999.0, "16G内存 512G固态", "TechPro"));

        List<Product> products = new ArrayList<>();
        for (ProductSpec spec : specs) {
            ProductFactory factory = factories.get(spec.getType());
            if (factory == null) {
                throw new IllegalStateException("未注册的商品工厂：" + spec.getType());
            }
            products.add(factory.createProduct(spec));
        }
        return products;
    }
}

interface DiscountStrategy {
    String getName();

    double applyDiscount(double originalTotal);
}

class PercentageDiscountStrategy implements DiscountStrategy {
    private final String name;
    private final double discountRate;

    public PercentageDiscountStrategy(String name, double discountRate) {
        if (discountRate <= 0 || discountRate > 1) {
            throw new IllegalArgumentException("折扣率必须在 0 到 1 之间");
        }
        this.name = name;
        this.discountRate = discountRate;
    }

    @Override
    public String getName() {
        return name;
    }

    @Override
    public double applyDiscount(double originalTotal) {
        return originalTotal * discountRate;
    }
}

class FullReductionDiscountStrategy implements DiscountStrategy {
    private final String name;
    private final double threshold;
    private final double reduction;

    public FullReductionDiscountStrategy(String name, double threshold, double reduction) {
        if (threshold <= 0 || reduction < 0) {
            throw new IllegalArgumentException("满减参数不合法");
        }
        this.name = name;
        this.threshold = threshold;
        this.reduction = reduction;
    }

    @Override
    public String getName() {
        return name;
    }

    @Override
    public double applyDiscount(double originalTotal) {
        if (originalTotal >= threshold) {
            return Math.max(0, originalTotal - reduction);
        }
        return originalTotal;
    }
}

class CompositeDiscountStrategy implements DiscountStrategy {
    private final String name;
    private final List<DiscountStrategy> strategies;

    public CompositeDiscountStrategy(String name, List<DiscountStrategy> strategies) {
        if (strategies == null || strategies.isEmpty()) {
            throw new IllegalArgumentException("组合策略不能为空");
        }
        this.name = name;
        this.strategies = new ArrayList<>(strategies);
    }

    @Override
    public String getName() {
        return name;
    }

    @Override
    public double applyDiscount(double originalTotal) {
        double currentTotal = originalTotal;
        for (DiscountStrategy strategy : strategies) {
            currentTotal = strategy.applyDiscount(currentTotal);
        }
        return currentTotal;
    }
}

final class DiscountPlan {
    private final String strategyName;
    private final double discountedTotal;
    private final double savedAmount;

    public DiscountPlan(String strategyName, double discountedTotal, double savedAmount) {
        this.strategyName = strategyName;
        this.discountedTotal = discountedTotal;
        this.savedAmount = savedAmount;
    }

    public String getStrategyName() {
        return strategyName;
    }

    public double getDiscountedTotal() {
        return discountedTotal;
    }

    public double getSavedAmount() {
        return savedAmount;
    }
}

class PromotionPlanner {
    public static DiscountPlan findBestPlan(double originalTotal, List<DiscountStrategy> strategies) {
        if (strategies == null || strategies.isEmpty()) {
            throw new IllegalArgumentException("至少要提供一个优惠策略");
        }

        DiscountPlan bestPlan = null;
        for (DiscountStrategy strategy : strategies) {
            double discountedTotal = strategy.applyDiscount(originalTotal);
            double savedAmount = originalTotal - discountedTotal;
            DiscountPlan currentPlan = new DiscountPlan(strategy.getName(), discountedTotal, savedAmount);
            if (bestPlan == null || currentPlan.getDiscountedTotal() < bestPlan.getDiscountedTotal()) {
                bestPlan = currentPlan;
            }
        }
        return bestPlan;
    }
}

class BasketPersistence {
    public static void saveBasket(GenericBasket<? extends Product> basket, Path filePath) throws IOException {
        String json = buildBasketJson(basket);
        Files.writeString(filePath, json, StandardCharsets.UTF_8);
    }

    public static GenericBasket<Product> loadBasket(Path filePath, int capacity) throws IOException {
        String json = Files.readString(filePath, StandardCharsets.UTF_8);
        int parsedCapacity = parseCapacity(json);
        GenericBasket<Product> basket = new GenericBasket<>(Math.max(capacity, parsedCapacity));
        for (String productJson : splitProductObjects(json)) {
            basket.addProduct(parseProduct(productJson));
        }
        return basket;
    }

    private static String buildBasketJson(GenericBasket<? extends Product> basket) {
        StringBuilder builder = new StringBuilder();
        builder.append("{\n");
        builder.append("  \"capacity\": ").append(basket.getCapacity()).append(",\n");
        builder.append("  \"products\": [\n");

        List<? extends Product> products = basket.getProductsSnapshot();
        for (int i = 0; i < products.size(); i++) {
            builder.append(buildProductJson(products.get(i)));
            if (i < products.size() - 1) {
                builder.append(",");
            }
            builder.append("\n");
        }
        builder.append("  ]\n");
        builder.append("}");
        return builder.toString();
    }

    private static String buildProductJson(Product product) {
        StringBuilder builder = new StringBuilder();
        builder.append("    {\n");
        builder.append("      \"type\": \"").append(escapeJson(resolveType(product))).append("\",\n");
        builder.append("      \"name\": \"").append(escapeJson(product.getName())).append("\",\n");
        builder.append("      \"price\": ").append(product.getPrice()).append(",\n");
        builder.append("      \"description\": \"").append(escapeJson(product.getDescription())).append("\"");

        if (product instanceof Food) {
            builder.append(",\n");
            builder.append("      \"expiryDate\": \"").append(escapeJson(product.getExtraValue())).append("\"\n");
        } else if (product instanceof Book) {
            builder.append(",\n");
            builder.append("      \"author\": \"").append(escapeJson(product.getExtraValue())).append("\"\n");
        } else if (product instanceof Electronics) {
            builder.append(",\n");
            builder.append("      \"brand\": \"").append(escapeJson(product.getExtraValue())).append("\"\n");
        } else {
            builder.append("\n");
        }
        builder.append("    }");
        return builder.toString();
    }

    private static Product parseProduct(String productJson) {
        String type = extractString(productJson, "type");
        String name = extractString(productJson, "name");
        double price = extractDouble(productJson, "price");
        String description = extractString(productJson, "description");

        switch (type) {
            case "Food":
                return new Food(name, price, description, LocalDate.parse(extractString(productJson, "expiryDate")));
            case "Book":
                return new Book(name, price, description, extractString(productJson, "author"));
            case "Electronics":
                return new Electronics(name, price, description, extractString(productJson, "brand"));
            default:
                throw new IllegalArgumentException("不支持的商品类型：" + type);
        }
    }

    private static int parseCapacity(String json) {
        Matcher matcher = Pattern.compile("\"capacity\"\\s*:\\s*(\\d+)").matcher(json);
        if (!matcher.find()) {
            throw new IllegalArgumentException("JSON 中缺少 capacity 字段");
        }
        return Integer.parseInt(matcher.group(1));
    }

    private static List<String> splitProductObjects(String json) {
        int productsKeyIndex = json.indexOf("\"products\"");
        if (productsKeyIndex < 0) {
            throw new IllegalArgumentException("JSON 中缺少 products 字段");
        }
        int arrayStart = json.indexOf("[", productsKeyIndex);
        int arrayEnd = json.lastIndexOf("]");
        if (arrayStart < 0 || arrayEnd < arrayStart) {
            throw new IllegalArgumentException("products 数组格式错误");
        }

        String arrayContent = json.substring(arrayStart + 1, arrayEnd);
        List<String> productObjects = new ArrayList<>();
        int braceDepth = 0;
        int objectStart = -1;

        for (int i = 0; i < arrayContent.length(); i++) {
            char currentChar = arrayContent.charAt(i);
            if (currentChar == '{') {
                if (braceDepth == 0) {
                    objectStart = i;
                }
                braceDepth++;
            } else if (currentChar == '}') {
                braceDepth--;
                if (braceDepth == 0 && objectStart >= 0) {
                    productObjects.add(arrayContent.substring(objectStart, i + 1));
                    objectStart = -1;
                }
            }
        }
        return productObjects;
    }

    private static String extractString(String json, String key) {
        Matcher matcher = Pattern.compile("\"" + Pattern.quote(key) + "\"\\s*:\\s*\"((?:\\\\.|[^\\\\\"])*)\"").matcher(json);
        if (!matcher.find()) {
            throw new IllegalArgumentException("JSON 中缺少字符串字段：" + key);
        }
        return unescapeJson(matcher.group(1));
    }

    private static double extractDouble(String json, String key) {
        Matcher matcher = Pattern.compile("\"" + Pattern.quote(key) + "\"\\s*:\\s*(-?\\d+(?:\\.\\d+)?)").matcher(json);
        if (!matcher.find()) {
            throw new IllegalArgumentException("JSON 中缺少数字字段：" + key);
        }
        return Double.parseDouble(matcher.group(1));
    }

    private static String resolveType(Product product) {
        if (product instanceof Food) {
            return "Food";
        }
        if (product instanceof Book) {
            return "Book";
        }
        if (product instanceof Electronics) {
            return "Electronics";
        }
        throw new IllegalArgumentException("未知商品类型：" + product.getClass().getSimpleName());
    }

    private static String escapeJson(String value) {
        return value
                .replace("\\", "\\\\")
                .replace("\"", "\\\"")
                .replace("\r", "\\r")
                .replace("\n", "\\n")
                .replace("\t", "\\t");
    }

    private static String unescapeJson(String value) {
        StringBuilder builder = new StringBuilder();
        boolean escaping = false;
        for (int i = 0; i < value.length(); i++) {
            char currentChar = value.charAt(i);
            if (escaping) {
                switch (currentChar) {
                    case '\\':
                        builder.append('\\');
                        break;
                    case '"':
                        builder.append('"');
                        break;
                    case 'n':
                        builder.append('\n');
                        break;
                    case 'r':
                        builder.append('\r');
                        break;
                    case 't':
                        builder.append('\t');
                        break;
                    default:
                        builder.append(currentChar);
                        break;
                }
                escaping = false;
            } else if (currentChar == '\\') {
                escaping = true;
            } else {
                builder.append(currentChar);
            }
        }
        if (escaping) {
            builder.append('\\');
        }
        return builder.toString();
    }
}

class BasketFullException extends RuntimeException {
    public BasketFullException(String message) {
        super(message);
    }
}

class BasketEmptyException extends RuntimeException {
    public BasketEmptyException(String message) {
        super(message);
    }
}
