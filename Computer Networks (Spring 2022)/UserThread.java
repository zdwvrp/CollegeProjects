import java.io.*;
import java.net.*;
import java.util.*;

public class UserThread extends Thread {

    private Socket socket;
    private Server server;
    private Scanner in;
    private PrintWriter out;

    // Constructor
    public UserThread( Socket socket, Server server ) {
        this.socket = socket;
        this.server = server;
    }

    // This function runs when a new instance of UserThread is instantiated.
    public void run() {
        try {

            // Initialize the socket's input and output streams and local variables.
            in = new Scanner(socket.getInputStream());
            out = new PrintWriter(socket.getOutputStream(), true);
            boolean loggedin = false;
            String command = "";
            String username = "";
            String password = "";

            // This loop is used to preserve the user's "un-logged-in" state until they have successfully logged in.
            while( !loggedin) {

                // Sends a LOGIN request to the Client.
                out.println("LOGIN");
                System.out.println("\nAwaiting login or newuser request from Client");

                // The Client's response is read and the command, username & password are saved into their respective local variables.
                String clientInput = in.nextLine();
                String[] strings = clientInput.split(" ", 3);
                command = strings[0];
                username = strings[1];
                password = strings[2];

                // Checks to see if the Client is attempting to login or create a new user.
                if(command.equals("login")) {
                    System.out.println("\nAttempting to login " + username);
                    loggedin = handleLogin(username, password);

                    // If the login information was valid, send a confirmation to the Client and add the user to the server.
                    if(loggedin) {
                        server.addUser(username);
                        System.out.println("\n" + username + " has logged in successfully");
                        out.println("GRANTED_LOGIN " + getWelcomeMessage(username));
                    // If the login was unsuccessful, the Client is notified.
                    // The wrapping while loop will then iterate, sending the Client another LOGIN request.
                    } else {
                        System.out.println("\n" + username + " has failed to login");
                        out.println("FAILED_LOGIN Server-Side Login Error: Your username or password is incorrect, please try again.");
                    }

                // Checks to see if the new user request has unique login information.
                // If yes, the user is created and a confirmation is sent to the Client.
                // If not, the user isn't created and the Client is notified.
                } else if(command.equals("newuser")) {
                    if(!handleNewUser(username, password)) {
                        System.out.println("\nUser " + username + " already exists, and hasn't been created");
                        out.println("USER_NOT_CREATED Server-Side NewUser Error: A user already exists with that username, please try again.");
                    } else {
                        server.addNewUser(username, password);
                        System.out.println("\nUser " + username + " has been created successfully");
                        out.println("NEW_USER_CREATED New User successfully created!");
                    }
                }
            }
 
            // Initialize variables to hold messages from the Client, and messages to be sent from the Server.
            String serverMessage, clientMessage, message;
            String[] strings;
 
            // This loop sends an EXPECTING_MESSAGE request to the Client repeatedly until the user logs out of the server.
            // After receiving a response from the Client, the user's username is added to the front of the message and sent back.
            do {
                out.println("EXPECTING_MESSAGE");
                System.out.println("\nAwaiting message from Client");

                // Client's message is split into the command portion and the message portion.
                clientMessage = in.nextLine();
                strings = clientMessage.split(" ", 2);
                command = strings[0];

                // If the Client was not requesting to logout, it must be sending a message. The message is received and relayed back to the Client.
                if( !command.equals("logout")) {

                    message = strings[1];
                    System.out.println("\nMessage received from Client: " + message);

                    serverMessage = "[" + username + "]: " + message;
                    server.sendMessage(serverMessage);
                    System.out.println("\nMessage has been successfully sent back to Client");
                }

            } while (!clientMessage.equals("logout"));
 
            // Remove the user from the server and close the socket.
            server.removeUser(username, this);
            socket.close();
            System.out.println("\n" + username + " has disconnected from the server.");
 
        } catch (IOException ex) {
            System.out.println("Server-Side Error: Error in UserThread - " + ex.getMessage());
            ex.printStackTrace();
        }
    }

    // Handles 'login' requests from the client.
    //
    // @param String username - Username of the user attempting to login.
    // @param String password - Password of the user attempting to login.
    // @return boolean - Returns true if the user's credentials exist on the
    //                   server, and false if they do not.
    boolean handleLogin( String username, String password ) {

        HashMap<String, String> users = server.getUserInfo();
        for(var user : users.keySet()) {
            if( username.equals(user) && password.equals(users.get(user)) ) {
                return true;
            }
        }

        return false;
    }

    // Handles 'newuser' requests from the client.
    //
    // @param String username - Username of the new user trying to be created.
    // @param String password - Password of the new user trying to be created.
    // @return boolean - Returns true if the requested new user doesn't already
    //                   exist, and false if the user's credentials already 
    //                   exist on the server.
    boolean handleNewUser( String username, String password ) {
        HashMap<String, String> users = server.getUserInfo();
        for(var user : users.keySet()) {
            if( username.equals(user) ) {
                return false;
            }
        }
        
        return true;
    }

    // @param String username - Username of the currently logged in user.
    // @return String - Returns the server's welcome message.
    String getWelcomeMessage(String username) {
        return "Login Successful, Welcome " + username + "! Type 'logout' when you're ready to disconnect!";
    }

    // Sends a message to the client.
    //
    // @param String message - Message to be sent to the client.
    void sendMessage(String message) {
        out.println("INCOMING_MESSAGE " + message);
    }
}