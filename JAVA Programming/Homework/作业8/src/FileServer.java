import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.EOFException;
import java.io.IOException;
import java.io.RandomAccessFile;
import java.net.ServerSocket;
import java.net.Socket;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.StandardOpenOption;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.List;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class FileServer {
    private static final int DEFAULT_PORT = 9000;
    private static final int BUFFER_SIZE = 8192;

    private final int port;
    private final Path rootDirectory;
    private final Path uploadRoot;
    private final Path sharedRoot;
    private final ExecutorService clientPool = Executors.newCachedThreadPool();

    public FileServer(int port, Path rootDirectory) {
        this.port = port;
        this.rootDirectory = rootDirectory;
        this.uploadRoot = rootDirectory.resolve("uploads");
        this.sharedRoot = rootDirectory.resolve("shared");
    }

    public static void main(String[] args) {
        int port = args.length > 0 ? Integer.parseInt(args[0]) : DEFAULT_PORT;
        Path root = args.length > 1 ? Paths.get(args[1]) : Paths.get("server-data");
        FileServer server = new FileServer(port, root);
        try {
            server.start();
        } catch (IOException e) {
            System.err.println("Failed to start server: " + e.getMessage());
        }
    }

    public void start() throws IOException {
        Files.createDirectories(uploadRoot);
        Files.createDirectories(sharedRoot);

        System.out.println("File server started on port " + port);
        System.out.println("Upload root : " + uploadRoot.toAbsolutePath());
        System.out.println("Shared root : " + sharedRoot.toAbsolutePath());

        try (ServerSocket serverSocket = new ServerSocket(port)) {
            while (true) {
                Socket socket = serverSocket.accept();
                clientPool.submit(new ClientHandler(socket));
            }
        }
    }

    private class ClientHandler implements Runnable {
        private final Socket socket;

        private ClientHandler(Socket socket) {
            this.socket = socket;
        }

        @Override
        public void run() {
            String remoteAddress = socket.getRemoteSocketAddress().toString();
            System.out.println("Client connected: " + remoteAddress);

            try (Socket client = socket;
                 DataInputStream in = new DataInputStream(new BufferedInputStream(client.getInputStream()));
                 DataOutputStream out = new DataOutputStream(new BufferedOutputStream(client.getOutputStream()))) {

                while (true) {
                    String command;
                    try {
                        command = in.readUTF();
                    } catch (EOFException eofException) {
                        break;
                    }

                    switch (command) {
                        case "LIST":
                            handleList(out);
                            break;
                        case "UPLOAD":
                            handleUpload(in, out);
                            break;
                        case "FILE_INFO":
                            handleFileInfo(in, out);
                            break;
                        case "DOWNLOAD":
                            handleDownload(in, out);
                            break;
                        case "DOWNLOAD_RANGE":
                            handleDownloadRange(in, out);
                            break;
                        case "QUIT":
                            out.writeUTF("BYE");
                            out.flush();
                            return;
                        default:
                            out.writeBoolean(false);
                            out.writeUTF("Unknown command: " + command);
                            out.flush();
                            break;
                    }
                }
            } catch (IOException e) {
                System.err.println("Client error (" + remoteAddress + "): " + e.getMessage());
            } finally {
                System.out.println("Client disconnected: " + remoteAddress);
            }
        }

        private void handleList(DataOutputStream out) throws IOException {
            List<Path> files = listSharedFiles();
            out.writeInt(files.size());
            for (Path file : files) {
                out.writeUTF(file.getFileName().toString());
                out.writeLong(Files.size(file));
            }
            out.flush();
        }

        private void handleUpload(DataInputStream in, DataOutputStream out) throws IOException {
            String clientId = sanitizeSegment(in.readUTF());
            String fileName = sanitizeFileName(in.readUTF());
            long fileSize = in.readLong();

            if (fileName.isEmpty() || fileSize < 0L) {
                out.writeBoolean(false);
                out.writeUTF("Invalid upload request.");
                out.flush();
                return;
            }

            Path clientDirectory = uploadRoot.resolve(clientId);
            Files.createDirectories(clientDirectory);

            Path targetFile = resolveUniquePath(clientDirectory, fileName);
            out.writeBoolean(true);
            out.writeUTF(targetFile.getFileName().toString());
            out.flush();

            ConsoleProgressBar progressBar = new ConsoleProgressBar("Receiving " + fileName, fileSize);
            try (BufferedOutputStream fileOut = new BufferedOutputStream(
                    Files.newOutputStream(targetFile, StandardOpenOption.CREATE_NEW))) {
                transferBytes(in, fileOut, fileSize, progressBar);
            }
            progressBar.finish();

            out.writeUTF("UPLOAD_OK");
            out.flush();
        }

        private void handleFileInfo(DataInputStream in, DataOutputStream out) throws IOException {
            String fileName = sanitizeFileName(in.readUTF());
            Path file = sharedRoot.resolve(fileName);
            if (fileName.isEmpty() || !Files.isRegularFile(file)) {
                out.writeBoolean(false);
                out.writeUTF("File not found.");
            } else {
                out.writeBoolean(true);
                out.writeLong(Files.size(file));
            }
            out.flush();
        }

        private void handleDownload(DataInputStream in, DataOutputStream out) throws IOException {
            String fileName = sanitizeFileName(in.readUTF());
            Path file = sharedRoot.resolve(fileName);
            if (fileName.isEmpty() || !Files.isRegularFile(file)) {
                out.writeBoolean(false);
                out.writeUTF("File not found.");
                out.flush();
                return;
            }

            long fileSize = Files.size(file);
            out.writeBoolean(true);
            out.writeLong(fileSize);
            out.flush();

            ConsoleProgressBar progressBar = new ConsoleProgressBar("Sending " + fileName, fileSize);
            try (BufferedInputStream fileIn = new BufferedInputStream(Files.newInputStream(file))) {
                transferBytes(fileIn, out, fileSize, progressBar);
            }
            progressBar.finish();
            out.flush();
        }

        private void handleDownloadRange(DataInputStream in, DataOutputStream out) throws IOException {
            String fileName = sanitizeFileName(in.readUTF());
            long start = in.readLong();
            long end = in.readLong();
            Path file = sharedRoot.resolve(fileName);

            if (fileName.isEmpty() || !Files.isRegularFile(file)) {
                out.writeBoolean(false);
                out.writeUTF("File not found.");
                out.flush();
                return;
            }

            long fileSize = Files.size(file);
            if (start < 0L || end < start || end >= fileSize) {
                out.writeBoolean(false);
                out.writeUTF("Invalid range.");
                out.flush();
                return;
            }

            long length = end - start + 1L;
            out.writeBoolean(true);
            out.writeLong(length);
            out.flush();

            byte[] buffer = new byte[BUFFER_SIZE];
            try (RandomAccessFile randomAccessFile = new RandomAccessFile(file.toFile(), "r")) {
                randomAccessFile.seek(start);
                long remaining = length;
                while (remaining > 0L) {
                    int bytesToRead = (int) Math.min(buffer.length, remaining);
                    int bytesRead = randomAccessFile.read(buffer, 0, bytesToRead);
                    if (bytesRead == -1) {
                        throw new IOException("Unexpected end of file while sending range.");
                    }
                    out.write(buffer, 0, bytesRead);
                    remaining -= bytesRead;
                }
            }
            out.flush();
        }
    }

    private List<Path> listSharedFiles() throws IOException {
        List<Path> files = new ArrayList<Path>();
        try (java.util.stream.Stream<Path> stream = Files.list(sharedRoot)) {
            stream.filter(Files::isRegularFile)
                    .sorted(Comparator.comparing(path -> path.getFileName().toString().toLowerCase()))
                    .forEach(files::add);
        }
        return files;
    }

    private Path resolveUniquePath(Path directory, String fileName) throws IOException {
        String baseName = fileName;
        String extension = "";
        int dotIndex = fileName.lastIndexOf('.');
        if (dotIndex > 0) {
            baseName = fileName.substring(0, dotIndex);
            extension = fileName.substring(dotIndex);
        }

        Path candidate = directory.resolve(fileName);
        int index = 1;
        while (Files.exists(candidate)) {
            candidate = directory.resolve(baseName + "_" + index + extension);
            index++;
        }
        return candidate;
    }

    private String sanitizeSegment(String value) {
        String sanitized = value == null ? "" : value.replaceAll("[^a-zA-Z0-9._-]", "_");
        return sanitized.isEmpty() ? "anonymous" : sanitized;
    }

    private String sanitizeFileName(String fileName) {
        if (fileName == null) {
            return "";
        }
        String normalized = Paths.get(fileName).getFileName().toString();
        if (".".equals(normalized) || "..".equals(normalized)) {
            return "";
        }
        return normalized.replaceAll("[\\\\/]", "_");
    }

    private void transferBytes(DataInputStream in, BufferedOutputStream out, long totalBytes,
                               ConsoleProgressBar progressBar) throws IOException {
        byte[] buffer = new byte[BUFFER_SIZE];
        long remaining = totalBytes;
        while (remaining > 0L) {
            int bytesToRead = (int) Math.min(buffer.length, remaining);
            int bytesRead = in.read(buffer, 0, bytesToRead);
            if (bytesRead == -1) {
                throw new IOException("Connection closed during transfer.");
            }
            out.write(buffer, 0, bytesRead);
            remaining -= bytesRead;
            progressBar.update(bytesRead);
        }
        out.flush();
    }

    private void transferBytes(BufferedInputStream in, DataOutputStream out, long totalBytes,
                               ConsoleProgressBar progressBar) throws IOException {
        byte[] buffer = new byte[BUFFER_SIZE];
        long remaining = totalBytes;
        while (remaining > 0L) {
            int bytesToRead = (int) Math.min(buffer.length, remaining);
            int bytesRead = in.read(buffer, 0, bytesToRead);
            if (bytesRead == -1) {
                throw new IOException("Unexpected end of file.");
            }
            out.write(buffer, 0, bytesRead);
            remaining -= bytesRead;
            progressBar.update(bytesRead);
        }
    }
}
