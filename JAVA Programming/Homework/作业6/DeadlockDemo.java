public class DeadlockDemo {
    private static final Object LOCK_A = new Object();
    private static final Object LOCK_B = new Object();

    public static void main(String[] args) throws InterruptedException {
        Thread t1 = new Thread(() -> {
            synchronized (LOCK_A) {
                System.out.println("Thread-1 got LOCK_A");
                sleepQuietly(500);
                System.out.println("Thread-1 waits for LOCK_B");
                synchronized (LOCK_B) {
                    System.out.println("Thread-1 got LOCK_B");
                }
            }
        });

        Thread t2 = new Thread(() -> {
            synchronized (LOCK_B) {
                System.out.println("Thread-2 got LOCK_B");
                sleepQuietly(500);
                System.out.println("Thread-2 waits for LOCK_A");
                synchronized (LOCK_A) {
                    System.out.println("Thread-2 got LOCK_A");
                }
            }
        });

        t1.start();
        t2.start();

        t1.join();
        t2.join();
    }

    private static void sleepQuietly(long millis) {
        try {
            Thread.sleep(millis);
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
    }
}
