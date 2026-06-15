import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.RenderingHints;
import java.util.Random;
import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.SwingConstants;
import javax.swing.SwingUtilities;

public class BubbleSortAnimation extends JFrame {
    private static final int ARRAY_SIZE = 20;

    private final int[] values = new int[ARRAY_SIZE];
    private final Random random = new Random();

    private final HistogramPanel histogramPanel = new HistogramPanel();
    private final JButton stepButton = new JButton("Step");
    private final JButton resetButton = new JButton("Reset");
    private final JLabel statusLabel = new JLabel("", SwingConstants.CENTER);

    private int currentPass;
    private int currentIndex;
    private int compareLeft = -1;
    private int compareRight = -1;
    private boolean finished;
    private boolean completionShown;

    public BubbleSortAnimation() {
        setTitle("冒泡排序动画");
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLayout(new BorderLayout(10, 10));

        statusLabel.setFont(new Font(Font.SANS_SERIF, Font.PLAIN, 14));
        statusLabel.setBorder(BorderFactory.createEmptyBorder(8, 8, 0, 8));

        JPanel buttonPanel = new JPanel();
        buttonPanel.add(stepButton);
        buttonPanel.add(resetButton);

        add(statusLabel, BorderLayout.NORTH);
        add(histogramPanel, BorderLayout.CENTER);
        add(buttonPanel, BorderLayout.SOUTH);

        stepButton.addActionListener(e -> performStep());
        resetButton.addActionListener(e -> resetAnimation());

        resetAnimation();

        pack();
        setLocationRelativeTo(null);
    }

    private void resetAnimation() {
        initializeRandomArray();
        currentPass = 0;
        currentIndex = 0;
        compareLeft = -1;
        compareRight = -1;
        finished = false;
        completionShown = false;
        stepButton.setEnabled(true);
        statusLabel.setText("单击 Step 执行一次比较，单击 Reset 重新生成随机数组。");
        histogramPanel.repaint();
    }

    private void initializeRandomArray() {
        for (int i = 0; i < ARRAY_SIZE; i++) {
            values[i] = i + 1;
        }

        for (int i = ARRAY_SIZE - 1; i > 0; i--) {
            int j = random.nextInt(i + 1);
            int temp = values[i];
            values[i] = values[j];
            values[j] = temp;
        }
    }

    private void performStep() {
        if (finished) {
            if (!completionShown) {
                showCompletionMessage();
            }
            return;
        }

        compareLeft = currentIndex;
        compareRight = currentIndex + 1;

        if (values[compareLeft] > values[compareRight]) {
            int temp = values[compareLeft];
            values[compareLeft] = values[compareRight];
            values[compareRight] = temp;
        }

        statusLabel.setText(
            "当前比较: 第 " + (compareLeft + 1) + " 个和第 " + (compareRight + 1) + " 个元素。"
        );

        currentIndex++;
        if (currentIndex >= ARRAY_SIZE - 1 - currentPass) {
            currentIndex = 0;
            currentPass++;
        }

        if (currentPass >= ARRAY_SIZE - 1) {
            finished = true;
            stepButton.setEnabled(false);
            compareLeft = -1;
            compareRight = -1;
            statusLabel.setText("排序完成，单击 Reset 可以重新开始。");
            showCompletionMessage();
        }

        histogramPanel.repaint();
    }

    private void showCompletionMessage() {
        completionShown = true;
        JOptionPane.showMessageDialog(
            this,
            "数组已经完成冒泡排序。",
            "提示",
            JOptionPane.INFORMATION_MESSAGE
        );
    }

    private class HistogramPanel extends JPanel {
        private HistogramPanel() {
            setPreferredSize(new Dimension(900, 420));
            setBackground(Color.WHITE);
        }

        @Override
        protected void paintComponent(Graphics g) {
            super.paintComponent(g);
            Graphics2D g2 = (Graphics2D) g.create();
            g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);

            int width = getWidth();
            int height = getHeight();
            int baseLine = height - 50;
            int chartHeight = height - 90;
            int barWidth = Math.max(24, (width - 60) / ARRAY_SIZE);
            int gap = 6;

            g2.setColor(Color.DARK_GRAY);
            g2.drawLine(30, baseLine, width - 20, baseLine);

            for (int i = 0; i < ARRAY_SIZE; i++) {
                int barHeight = values[i] * chartHeight / ARRAY_SIZE;
                int x = 30 + i * barWidth;
                int y = baseLine - barHeight;
                int drawWidth = barWidth - gap;

                if (i == compareLeft || i == compareRight) {
                    g2.setColor(new Color(220, 50, 47));
                } else if (i >= ARRAY_SIZE - currentPass && currentPass > 0) {
                    g2.setColor(new Color(46, 204, 113));
                } else {
                    g2.setColor(new Color(52, 152, 219));
                }

                g2.fillRect(x, y, drawWidth, barHeight);
                g2.setColor(Color.BLACK);
                g2.drawRect(x, y, drawWidth, barHeight);
                g2.drawString(String.valueOf(values[i]), x + drawWidth / 2 - 6, y - 6);
            }

            g2.dispose();
        }
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> {
            BubbleSortAnimation frame = new BubbleSortAnimation();
            frame.setVisible(true);
        });
    }
}
