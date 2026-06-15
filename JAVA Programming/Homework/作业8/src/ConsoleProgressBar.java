import java.text.DecimalFormat;
import java.util.concurrent.atomic.AtomicLong;

public class ConsoleProgressBar {
    private static final int BAR_WIDTH = 30;
    private static final long RENDER_INTERVAL_MS = 120L;

    private final String label;
    private final long totalBytes;
    private final AtomicLong transferredBytes = new AtomicLong(0L);
    private final DecimalFormat decimalFormat = new DecimalFormat("0.0");

    private volatile long lastRenderTime = 0L;

    public ConsoleProgressBar(String label, long totalBytes) {
        this.label = label;
        this.totalBytes = Math.max(totalBytes, 1L);
    }

    public void update(long delta) {
        transferredBytes.addAndGet(delta);
        render(false);
    }

    public synchronized void finish() {
        render(true);
        System.out.println();
    }

    private synchronized void render(boolean force) {
        long now = System.currentTimeMillis();
        if (!force && now - lastRenderTime < RENDER_INTERVAL_MS) {
            return;
        }
        lastRenderTime = now;

        long current = Math.min(transferredBytes.get(), totalBytes);
        double ratio = (double) current / (double) totalBytes;
        int filled = (int) Math.round(ratio * BAR_WIDTH);
        StringBuilder bar = new StringBuilder(BAR_WIDTH);
        for (int i = 0; i < BAR_WIDTH; i++) {
            bar.append(i < filled ? '=' : ' ');
        }

        String line = String.format(
                "\r%s [%s] %3d%% %s/%s",
                label,
                bar,
                (int) (ratio * 100),
                formatBytes(current),
                formatBytes(totalBytes)
        );
        System.out.print(line);
    }

    private String formatBytes(long bytes) {
        if (bytes < 1024L) {
            return bytes + " B";
        }
        double kb = bytes / 1024.0;
        if (kb < 1024.0) {
            return decimalFormat.format(kb) + " KB";
        }
        double mb = kb / 1024.0;
        if (mb < 1024.0) {
            return decimalFormat.format(mb) + " MB";
        }
        double gb = mb / 1024.0;
        return decimalFormat.format(gb) + " GB";
    }
}
