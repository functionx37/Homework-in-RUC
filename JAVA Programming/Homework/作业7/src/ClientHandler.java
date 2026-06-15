import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.EOFException;
import java.io.IOException;
import java.net.Socket;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.List;
import java.util.UUID;

public class ClientHandler implements Runnable {
    private static final int BUFFER_SIZE = 8192;

    private final Socket socket;
    private final String clientId;
    private final Path clientUploadDir;

    public ClientHandler(Socket socket) throws IOException {
        this.socket = socket;
        this.clientId = "client_" + UUID.randomUUID().toString().replace("-", "").substring(0, 8);
        this.clientUploadDir = ServerMain.UPLOAD_ROOT.resolve(clientId);
        Files.createDirectories(clientUploadDir);
    }

    @Override
    public void run() {
        System.out.println("客户端已连接: " + clientId + " " + socket.getRemoteSocketAddress());
        try (Socket autoCloseSocket = socket;
             DataInputStream input = new DataInputStream(new BufferedInputStream(autoCloseSocket.getInputStream()));
             DataOutputStream output = new DataOutputStream(new BufferedOutputStream(autoCloseSocket.getOutputStream()))) {

            output.writeUTF("CONNECTED");
            output.writeUTF(clientId);
            output.flush();

            while (true) {
                String command;
                try {
                    command = input.readUTF();
                } catch (EOFException e) {
                    break;
                }

                if ("UPLOAD".equalsIgnoreCase(command)) {
                    handleUpload(input, output);
                } else if ("LIST".equalsIgnoreCase(command)) {
                    handleList(output);
                } else if ("DOWNLOAD".equalsIgnoreCase(command)) {
                    handleDownload(input, output);
                } else if ("QUIT".equalsIgnoreCase(command)) {
                    output.writeUTF("BYE");
                    output.flush();
                    break;
                } else {
                    output.writeUTF("ERROR");
                    output.writeUTF("未知命令: " + command);
                    output.flush();
                }
            }
        } catch (IOException e) {
            System.out.println("客户端连接异常 " + clientId + ": " + e.getMessage());
        } finally {
            System.out.println("客户端已断开: " + clientId);
        }
    }

    private void handleUpload(DataInputStream input, DataOutputStream output) throws IOException {
        String fileName = input.readUTF();
        long fileSize = input.readLong();
        Path targetFile = clientUploadDir.resolve(Path.of(fileName).getFileName().toString());

        try (BufferedOutputStream fileOutput = new BufferedOutputStream(Files.newOutputStream(targetFile))) {
            transferToFile(input, fileOutput, fileSize);
        }

        output.writeUTF("OK");
        output.writeUTF("上传成功，文件已保存到: " + targetFile.toAbsolutePath());
        output.flush();
        System.out.println(clientId + " 上传文件: " + targetFile.getFileName() + " (" + fileSize + " 字节)");
    }

    private void handleList(DataOutputStream output) throws IOException {
        List<Path> fileList;
        try (var stream = Files.list(ServerMain.SHARED_ROOT)) {
            fileList = stream
                    .filter(Files::isRegularFile)
                    .sorted()
                    .toList();
        }

        output.writeUTF("OK");
        output.writeInt(fileList.size());
        for (Path path : fileList) {
            output.writeUTF(path.getFileName().toString());
            output.writeLong(Files.size(path));
        }
        output.flush();
    }

    private void handleDownload(DataInputStream input, DataOutputStream output) throws IOException {
        String requestedName = input.readUTF();
        Path target = ServerMain.SHARED_ROOT.resolve(Path.of(requestedName).getFileName().toString());

        if (!Files.exists(target) || !Files.isRegularFile(target)) {
            output.writeUTF("ERROR");
            output.writeUTF("服务器共享目录中不存在该文件: " + requestedName);
            output.flush();
            return;
        }

        long size = Files.size(target);
        output.writeUTF("OK");
        output.writeUTF(target.getFileName().toString());
        output.writeLong(size);

        try (BufferedInputStream fileInput = new BufferedInputStream(Files.newInputStream(target))) {
            transferFromFile(fileInput, output, size);
        }
        output.flush();
        System.out.println(clientId + " 下载文件: " + target.getFileName() + " (" + size + " 字节)");
    }

    private void transferToFile(DataInputStream input, BufferedOutputStream output, long expectedSize) throws IOException {
        byte[] buffer = new byte[BUFFER_SIZE];
        long remaining = expectedSize;
        while (remaining > 0) {
            int readSize = (int) Math.min(buffer.length, remaining);
            int bytesRead = input.read(buffer, 0, readSize);
            if (bytesRead == -1) {
                throw new EOFException("上传过程中连接中断");
            }
            output.write(buffer, 0, bytesRead);
            remaining -= bytesRead;
        }
        output.flush();
    }

    private void transferFromFile(BufferedInputStream input, DataOutputStream output, long size) throws IOException {
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
}
