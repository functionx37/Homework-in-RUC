import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class ServerMain {
    public static final int DEFAULT_PORT = 5000;
    public static final Path STORAGE_ROOT = Paths.get("server_storage");
    public static final Path UPLOAD_ROOT = STORAGE_ROOT.resolve("uploads");
    public static final Path SHARED_ROOT = STORAGE_ROOT.resolve("shared");

    public static void main(String[] args) {
        int port = DEFAULT_PORT;
        if (args.length > 0) {
            try {
                port = Integer.parseInt(args[0]);
            } catch (NumberFormatException e) {
                System.out.println("端口号格式错误，使用默认端口 " + DEFAULT_PORT);
            }
        }

        try {
            Files.createDirectories(UPLOAD_ROOT);
            Files.createDirectories(SHARED_ROOT);
        } catch (IOException e) {
            System.out.println("初始化服务器目录失败: " + e.getMessage());
            return;
        }

        ExecutorService pool = Executors.newCachedThreadPool();
        try (ServerSocket serverSocket = new ServerSocket(port)) {
            System.out.println("文件服务器已启动，监听端口: " + port);
            System.out.println("共享下载目录: " + SHARED_ROOT.toAbsolutePath());

            while (true) {
                Socket socket = serverSocket.accept();
                pool.execute(new ClientHandler(socket));
            }
        } catch (IOException e) {
            System.out.println("服务器运行出错: " + e.getMessage());
        } finally {
            pool.shutdown();
        }
    }
}
