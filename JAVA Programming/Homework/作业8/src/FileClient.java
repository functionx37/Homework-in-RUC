import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.RandomAccessFile;
import java.net.Socket;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.StandardOpenOption;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;
import java.util.concurrent.Callable;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;

public class FileClient {
    private static final int DEFAULT_PORT = 9000;
    private static final int DEFAULT_DOWNLOAD_THREADS = 4;
    private static final int BUFFER_SIZE = 8192;

    private final String host;
    private final int port;
    private final String clientId;
    private final Path downloadRoot;

    public FileClient(String host, int port, String clientId, Path downloadRoot) {
        this.host = host;
        this.port = port;
        this.clientId = clientId;
        this.downloadRoot = downloadRoot;
    }

    public static void main(String[] args) {
        String host = args.length > 0 ? args[0] : "127.0.0.1";
        int port = args.length > 1 ? Integer.parseInt(args[1]) : DEFAULT_PORT;
        String clientId = args.length > 2 ? args[2] : "client1";
        Path downloadRoot = args.length > 3 ? Paths.get(args[3]) : Paths.get("client-downloads", clientId);

        FileClient client = new FileClient(host, port, clientId, downloadRoot);
        try {
            client.startConsole();
        } catch (IOException e) {
            System.err.println("Client error: " + e.getMessage());
        }
    }

    public void startConsole() throws IOException {
        Files.createDirectories(downloadRoot);
        printHelp();

        try (Scanner scanner = new Scanner(System.in)) {
            while (true) {
                System.out.print("\nclient> ");
                if (!scanner.hasNextLine()) {
                    break;
                }

                String line = scanner.nextLine().trim();
                if (line.isEmpty()) {
                    continue;
                }

                try {
                    if ("help".equalsIgnoreCase(line)) {
                        printHelp();
                    } else if ("list".equalsIgnoreCase(line)) {
                        listFiles();
                    } else if (line.startsWith("upload ")) {
                        upload(Paths.get(line.substring("upload ".length()).trim()));
                    } else if (line.startsWith("download ")) {
                        handleDownloadCommand(line);
                    } else if ("exit".equalsIgnoreCase(line) || "quit".equalsIgnoreCase(line)) {
                        System.out.println("Bye.");
                        break;
                    } else {
                        System.out.println("Unknown command. Type 'help' to view usage.");
                    }
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                    throw new IOException("Client interrupted.", e);
                } catch (IOException | ExecutionException e) {
                    System.err.println("Operation failed: " + e.getMessage());
                } catch (NumberFormatException e) {
                    System.err.println("Invalid command format. Thread count must be a number.");
                }
            }
        }
    }

    private void handleDownloadCommand(String line) throws IOException, ExecutionException, InterruptedException {
        String[] parts = line.split("\\s+");
        if (parts.length < 2) {
            System.out.println("Usage: download <remote-file> [thread-count]");
            return;
        }

        String remoteFile = parts[1];
        int threadCount = parts.length >= 3 ? Integer.parseInt(parts[2]) : DEFAULT_DOWNLOAD_THREADS;
        threadCount = Math.max(threadCount, 1);

        if (threadCount == 1) {
            download(remoteFile);
        } else {
            downloadMultiThread(remoteFile, threadCount);
        }
    }

    public void listFiles() throws IOException {
        try (Connection connection = openConnection()) {
            connection.out.writeUTF("LIST");
            connection.out.flush();

            int count = connection.in.readInt();
            if (count == 0) {
                System.out.println("No files available in the shared directory.");
                return;
            }

            System.out.println("Shared files:");
            for (int i = 0; i < count; i++) {
                String fileName = connection.in.readUTF();
                long size = connection.in.readLong();
                System.out.println(" - " + fileName + " (" + size + " bytes)");
            }
        }
    }

    public void upload(Path localFile) throws IOException {
        if (!Files.isRegularFile(localFile)) {
            throw new IOException("Local file does not exist: " + localFile);
        }

        long fileSize = Files.size(localFile);
        try (Connection connection = openConnection();
             BufferedInputStream fileIn = new BufferedInputStream(Files.newInputStream(localFile))) {

            connection.out.writeUTF("UPLOAD");
            connection.out.writeUTF(clientId);
            connection.out.writeUTF(localFile.getFileName().toString());
            connection.out.writeLong(fileSize);
            connection.out.flush();

            boolean accepted = connection.in.readBoolean();
            if (!accepted) {
                throw new IOException(connection.in.readUTF());
            }

            String storedName = connection.in.readUTF();
            ConsoleProgressBar progressBar = new ConsoleProgressBar("Uploading " + storedName, fileSize);
            transferBytes(fileIn, connection.out, fileSize, progressBar);
            connection.out.flush();
            progressBar.finish();

            String result = connection.in.readUTF();
            System.out.println("Server response: " + result);
        }
    }

    public void download(String remoteFile) throws IOException {
        try (Connection connection = openConnection()) {
            connection.out.writeUTF("DOWNLOAD");
            connection.out.writeUTF(remoteFile);
            connection.out.flush();

            boolean exists = connection.in.readBoolean();
            if (!exists) {
                throw new IOException(connection.in.readUTF());
            }

            long fileSize = connection.in.readLong();
            Path target = resolveUniqueDownloadPath(remoteFile);

            ConsoleProgressBar progressBar = new ConsoleProgressBar("Downloading " + remoteFile, fileSize);
            try (BufferedOutputStream fileOut = new BufferedOutputStream(
                    Files.newOutputStream(target, StandardOpenOption.CREATE_NEW))) {
                transferBytes(connection.in, fileOut, fileSize, progressBar);
            }
            progressBar.finish();
            System.out.println("Saved to: " + target.toAbsolutePath());
        }
    }

    public void downloadMultiThread(String remoteFile, int threadCount)
            throws IOException, ExecutionException, InterruptedException {
        long fileSize = requestFileSize(remoteFile);
        if (fileSize <= 0L) {
            throw new IOException("File is empty or unavailable: " + remoteFile);
        }

        Path target = resolveUniqueDownloadPath(remoteFile);
        Files.createDirectories(target.getParent());
        try (RandomAccessFile randomAccessFile = new RandomAccessFile(target.toFile(), "rw")) {
            randomAccessFile.setLength(fileSize);
        }

        ConsoleProgressBar progressBar = new ConsoleProgressBar(
                "Downloading " + remoteFile + " x" + threadCount, fileSize);
        ExecutorService executorService = Executors.newFixedThreadPool(threadCount);
        List<Future<Void>> futures = new ArrayList<Future<Void>>();

        long chunkSize = (fileSize + threadCount - 1L) / threadCount;
        for (int i = 0; i < threadCount; i++) {
            final long start = i * chunkSize;
            if (start >= fileSize) {
                break;
            }
            final long end = Math.min(fileSize - 1L, start + chunkSize - 1L);
            futures.add(executorService.submit(new RangeDownloadTask(remoteFile, target, start, end, progressBar)));
        }

        try {
            for (Future<Void> future : futures) {
                future.get();
            }
            progressBar.finish();
            System.out.println("Saved to: " + target.toAbsolutePath());
        } catch (ExecutionException | InterruptedException e) {
            Files.deleteIfExists(target);
            throw e;
        } finally {
            executorService.shutdownNow();
        }
    }

    private long requestFileSize(String remoteFile) throws IOException {
        try (Connection connection = openConnection()) {
            connection.out.writeUTF("FILE_INFO");
            connection.out.writeUTF(remoteFile);
            connection.out.flush();

            boolean exists = connection.in.readBoolean();
            if (!exists) {
                throw new IOException(connection.in.readUTF());
            }
            return connection.in.readLong();
        }
    }

    private Path resolveUniqueDownloadPath(String remoteFile) throws IOException {
        String fileName = Paths.get(remoteFile).getFileName().toString();
        Files.createDirectories(downloadRoot);

        String baseName = fileName;
        String extension = "";
        int dotIndex = fileName.lastIndexOf('.');
        if (dotIndex > 0) {
            baseName = fileName.substring(0, dotIndex);
            extension = fileName.substring(dotIndex);
        }

        Path candidate = downloadRoot.resolve(fileName);
        int index = 1;
        while (Files.exists(candidate)) {
            candidate = downloadRoot.resolve(baseName + "_" + index + extension);
            index++;
        }
        return candidate;
    }

    private Connection openConnection() throws IOException {
        Socket socket = new Socket(host, port);
        return new Connection(socket);
    }

    private void transferBytes(BufferedInputStream in, DataOutputStream out, long totalBytes,
                               ConsoleProgressBar progressBar) throws IOException {
        byte[] buffer = new byte[BUFFER_SIZE];
        long remaining = totalBytes;
        while (remaining > 0L) {
            int bytesToRead = (int) Math.min(buffer.length, remaining);
            int bytesRead = in.read(buffer, 0, bytesToRead);
            if (bytesRead == -1) {
                throw new IOException("Unexpected end of local file.");
            }
            out.write(buffer, 0, bytesRead);
            remaining -= bytesRead;
            progressBar.update(bytesRead);
        }
    }

    private void transferBytes(DataInputStream in, BufferedOutputStream out, long totalBytes,
                               ConsoleProgressBar progressBar) throws IOException {
        byte[] buffer = new byte[BUFFER_SIZE];
        long remaining = totalBytes;
        while (remaining > 0L) {
            int bytesToRead = (int) Math.min(buffer.length, remaining);
            int bytesRead = in.read(buffer, 0, bytesToRead);
            if (bytesRead == -1) {
                throw new IOException("Connection closed during download.");
            }
            out.write(buffer, 0, bytesRead);
            remaining -= bytesRead;
            progressBar.update(bytesRead);
        }
        out.flush();
    }

    private void printHelp() {
        System.out.println("Commands:");
        System.out.println("  list");
        System.out.println("  upload <local-file>");
        System.out.println("  download <remote-file> [thread-count]");
        System.out.println("  help");
        System.out.println("  exit");
    }

    private class RangeDownloadTask implements Callable<Void> {
        private final String remoteFile;
        private final Path target;
        private final long start;
        private final long end;
        private final ConsoleProgressBar progressBar;

        private RangeDownloadTask(String remoteFile, Path target, long start, long end,
                                  ConsoleProgressBar progressBar) {
            this.remoteFile = remoteFile;
            this.target = target;
            this.start = start;
            this.end = end;
            this.progressBar = progressBar;
        }

        @Override
        public Void call() throws Exception {
            try (Connection connection = openConnection();
                 RandomAccessFile file = new RandomAccessFile(target.toFile(), "rw")) {
                connection.out.writeUTF("DOWNLOAD_RANGE");
                connection.out.writeUTF(remoteFile);
                connection.out.writeLong(start);
                connection.out.writeLong(end);
                connection.out.flush();

                boolean success = connection.in.readBoolean();
                if (!success) {
                    throw new IOException(connection.in.readUTF());
                }

                long expectedLength = connection.in.readLong();
                long remaining = expectedLength;
                byte[] buffer = new byte[BUFFER_SIZE];
                file.seek(start);

                while (remaining > 0L) {
                    int bytesToRead = (int) Math.min(buffer.length, remaining);
                    int bytesRead = connection.in.read(buffer, 0, bytesToRead);
                    if (bytesRead == -1) {
                        throw new IOException("Connection closed during ranged download.");
                    }
                    file.write(buffer, 0, bytesRead);
                    remaining -= bytesRead;
                    progressBar.update(bytesRead);
                }
            }
            return null;
        }
    }

    private static class Connection implements AutoCloseable {
        private final Socket socket;
        private final DataInputStream in;
        private final DataOutputStream out;

        private Connection(Socket socket) throws IOException {
            this.socket = socket;
            this.in = new DataInputStream(new BufferedInputStream(socket.getInputStream()));
            this.out = new DataOutputStream(new BufferedOutputStream(socket.getOutputStream()));
        }

        @Override
        public void close() throws IOException {
            socket.close();
        }
    }
}
