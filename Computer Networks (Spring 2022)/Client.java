import java.net.*;
import java.io.*;

public class Client {
    private String hostname;
    private int port;
    private String username;
 
    // Constructor
    public Client(String hostname, int port) {
        this.hostname = hostname;
        this.port = port;
    }

    public static void main(String[] args) {
        if (args.length < 2) return;
 
        // Fetch the hostname and port from arguments and create a Client instance using them.
        String hostname = args[0];
        int port = Integer.parseInt(args[1]);
 
        Client client = new Client(hostname, port);
        client.start();
    }
 
    public void start() {
        try {
            Socket socket = new Socket(hostname, port);
 
            System.out.println("---Client has successfully connected to the chat server---");

            new ClientThread(socket, this).start();
 
        } catch (UnknownHostException ex) {
            System.out.println("Error: Server not found - " + ex.getMessage());
        } catch (IOException ex) {
            System.out.println("Error: I/O Error - " + ex.getMessage());
        }
 
    }
 
    // Sets the username of the Client's user.
    void setUsername(String username) {
        this.username = username;
    }
 
    // Returns the username of the Client's user.
    String getUsername() {
        return this.username;
    }
}
