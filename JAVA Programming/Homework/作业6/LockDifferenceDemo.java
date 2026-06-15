public class LockDifferenceDemo {
    public static void main(String[] args) throws InterruptedException {
        System.out.println("=== Same instance: synchronized instance method ===");
        Counter instanceA = new Counter("A");
        runAndWait(
                new Thread(() -> instanceA.instanceTask("T1")),
                new Thread(() -> instanceA.instanceTask("T2"))
        );

        System.out.println("\n=== Different instances: synchronized instance method ===");
        Counter instanceB = new Counter("B");
        Counter instanceC = new Counter("C");
        runAndWait(
                new Thread(() -> instanceB.instanceTask("T3")),
                new Thread(() -> instanceC.instanceTask("T4"))
        );

        System.out.println("\n=== Static synchronized method ===");
        runAndWait(
                new Thread(() -> Counter.staticTask("T5")),
                new Thread(() -> Counter.staticTask("T6"))
        );
    }

    private static void runAndWait(Thread first, Thread second) throws InterruptedException {
        first.start();
        second.start();
        first.join();
        second.join();
    }

    private static final class Counter {
        private final String name;

        private Counter(String name) {
            this.name = name;
        }

        public synchronized void instanceTask(String threadName) {
            System.out.println(threadName + " enters instance lock of object " + name);
            sleepQuietly(1000);
            System.out.println(threadName + " leaves instance lock of object " + name);
        }

        public static synchronized void staticTask(String threadName) {
            System.out.println(threadName + " enters class lock");
            sleepQuietly(1000);
            System.out.println(threadName + " leaves class lock");
        }

        private static void sleepQuietly(long millis) {
            try {
                Thread.sleep(millis);
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
        }
    }
}
