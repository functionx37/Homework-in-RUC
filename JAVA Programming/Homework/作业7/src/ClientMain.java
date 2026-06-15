import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.EOFException;
import java.io.IOException;
import java.net.Socket;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.Scanner;

public class ClientMain {
    private static final int BUFFER_SIZE = 8192;

    public static void main(String[] args) {
        String host = args.length > 0 ? args[0] : "127.0.0.1";
        int port = args.length > 1 ? parsePort(args[1]) : ServerMain.DEFAULT_PORT;
        Path downloadDir = Paths.get("client_downloads");

        try {
            Files.createDirectories(downloadDir);
        } catch (IOException e) {
            System.out.println("创建客户端下载目录失败: " + e.getMessage());
            return;
        }

        try (Socket socket = new Socket(host, port);
             DataInputStream input = new DataInputStream(new BufferedInputStream(socket.getInputStream()));
             DataOutputStream output = new DataOutputStream(new BufferedOutputStream(socket.getOutputStream()));
             Scanner scanner = new Scanner(System.in)) {

            String status = input.readUTF();
            String clientId = input.readUTF();
            if (!"CONNECTED".equals(status)) {
                System.out.println("连接服务器失败。");
                return;
            }

            System.out.println("已连接服务器，客户端编号: " + clientId);
            System.out.println("可用命令: upload, list, download, quit");

            while (true) {
                System.out.print("请输入命令: ");
                String command = scanner.nextLine().trim().toLowerCase();

                if ("upload".equals(command)) {
                    uploadFile(scanner, input, output);
                } else if ("list".equals(command)) {
                    listFiles(input, output);
                } else if ("download".equals(command)) {
                    downloadFile(scanner, input, output, downloadDir);
                } else if ("quit".equals(command)) {
                    output.writeUTF("QUIT");
                    output.flush();
                    System.out.println(input.readUTF());
                    break;
                } else if (!command.isEmpty()) {
                    System.out.println("不支持的命令，请输入 upload/list/download/quit。");
                }
            }
        } catch (EOFException e) {
            System.out.println("服务器已断开连接。");
        } catch (IOException e) {
            System.out.println("客户端运行出错: " + e.getMessage());
        }
    }

    private static int parsePort(String portText) {
        try {
            return Integer.parseInt(portText);
        } catch (NumberFormatException e) {
            System.out.println("端口号格式错误，使用默认端口 " + ServerMain.DEFAULT_PORT);
            return ServerMain.DEFAULT_PORT;
        }
    }

    private static void uploadFile(Scanner scanner, DataInputStream input, DataOutputStream output) throws IOException {
        System.out.print("请输入要上传的本地文件路径: ");
        String filePathText = scanner.nextLine().trim();
        Path filePath = Paths.get(filePathText);

        if (!Files.exists(filePath) || !Files.isRegularFile(filePath)) {
            System.out.println("文件不存在，上传取消。");
            return;
        }

        long fileSize = Files.size(filePath);
        output.writeUTF("UPLOAD");
        output.writeUTF(filePath.getFileName().toString());
        output.writeLong(fileSize);

        try (BufferedInputStream fileInput = new BufferedInputStream(Files.newInputStream(filePath))) {
            transferFromFile(fileInput, output, fileSize);
        }
        output.flush();

        String result = input.readUTF();
        String message = input.readUTF();
        System.out.println(result + ": " + message);
    }

    private static void listFiles(DataInputStream input, DataOutputStream output) throws IOException {
        output.writeUTF("LIST");
        output.flush();

        String result = input.readUTF();
        if (!"OK".equals(result)) {
            System.out.println("获取文件列表失败。");
            return;
        }

        int fileCount = input.readInt();
        if (fileCount == 0) {
            System.out.println("服务器共享目录暂无可下载文件。");
            return;
        }

        System.out.println("服务器共享目录文件列表:");
        for (int i = 0; i < fileCount; i++) {
            String name = input.readUTF();
            long size = input.readLong();
            System.out.println((i + 1) + ". " + name + " (" + size + " 字节)");
        }
    }

    private static void downloadFile(
            Scanner scanner,
            DataInputStream input,
            DataOutputStream output,
            Path downloadDir
    ) throws IOException {
        System.out.print("请输入要下载的服务器文件名: ");
        String requestedName = scanner.nextLine().trim();
        if (requestedName.isEmpty()) {
            System.out.println("文件名不能为空。");
            return;
        }

        output.writeUTF("DOWNLOAD");
        output.writeUTF(requestedName);
        output.flush();

        String result = input.readUTF();
        if (!"OK".equals(result)) {
            System.out.println(input.readUTF());
            return;
        }

        String fileName = input.readUTF();
        long fileSize = input.readLong();
        Path localTarget = downloadDir.resolve(fileName);

        try (BufferedOutputStream fileOutput = new BufferedOutputStream(Files.newOutputStream(localTarget))) {
            transferToFile(input, fileOutput, fileSize);
        }

        System.out.println("下载完成，文件已保存到: " + localTarget.toAbsolutePath());
    }

    private static void transferFromFile(BufferedInputStream input, DataOutputStream output, long size) throws IOException {
        byte[] buffer = new byte[BUFFER_SIZE];
        long sent = 0;
        while (sent < size) {
            int bytesRead = input.read(buffer);
            if (bytesRead == -1) {
                break;
            }
            output.write(buffer, 0, bytesRead);
            sent += bytesRead;
        }
    }

    private static void transferToFile(DataInputStream input, BufferedOutputStream output, long expectedSize) throws IOException {
        byte[] buffer = new byte[BUFFER_SIZE];
        long remaining = expectedSize;
        while (remaining > 0) {
            int readSize = (int) Math.min(buffer.length, remaining);
            int bytesRead = input.read(buffer, 0, readSize);
            if (bytesRead == -1) {
                throw new EOFException("下载过程中连接中断");
            }
            output.write(buffer, 0, bytesRead);
            remaining -= bytesRead;
        }
        output.flush();
    }
}
