import java.io.*;
import java.net.*;
import java.util.*;
 
/**
 * This thread is responsible for reading user's input and send it
 * to the server.
 * It runs in an infinite loop until the user types 'bye' to quit.
 */
public class ClientThread extends Thread {
    private Scanner in;
    private PrintWriter out;
    private Socket socket;
    private Client client;
 
    public ClientThread(Socket socket, Client client) {
        this.socket = socket;
        this.client = client;
 
        try {
            in = new Scanner(socket.getInputStream());
            out = new PrintWriter(socket.getOutputStream(), true);
        } catch (IOException ex) {
            System.out.println("Client-Side Error: Error getting I/O stream - " + ex.getMessage());
            ex.printStackTrace();
        }
    }
 
    public void run() {
 
        Console console = System.console();

        System.out.println("\nWelcome to the chatroom!");
        System.out.println("\nIf you already have an account, type 'login' followed by your username and password.");
        System.out.println("\nIf you'd like to create an account, type 'newuser' followed by the username and password you'd like to use.");
        System.out.println("\n*NOTE* When creating an account, the username must have a length of 3-32 characters, and the password must have a length of 4-8 characters.");

        while( in.hasNextLine() ) {
            String serverInput = in.nextLine();
            String username = "";

            if(serverInput.startsWith("LOGIN")) {
                username = login(console, out);

            } else if(serverInput.startsWith("GRANTED_LOGIN")) {
                System.out.println("\n" + serverInput.substring(14));
                client.setUsername(username);

            } else if(serverInput.startsWith("FAILED_LOGIN")) {
                System.out.println("\n" + serverInput.substring(13));

            } else if(serverInput.startsWith("NEW_USER_CREATED")) {
                System.out.println("\n" + serverInput.substring(17));
            
            } else if(serverInput.startsWith("USER_NOT_CREATED")) {
                System.out.println("\n" + serverInput.substring(17));
            
            } else if(serverInput.startsWith("EXPECTING_MESSAGE")) {
                messager(console, username);

            } else if(serverInput.startsWith("INCOMING_MESSAGE")) {
                System.out.println("\n" + serverInput.substring(17));
            }
        }
    }

    // This function is used to handle an initial user that hasn't logged in yet.
    // @param Console console - Initialized console object.
    // @param Printwriter out - Initialized Printwriter used to send data to the server.
    // @return String - Returns the user's username input upon a successful use of either 'login' or 'newuser'.
    private String login(Console console, PrintWriter out) {

        boolean validRequest = false;
        String command = "";
        String username = "";
        String password = "";

        while(!validRequest) {

            // These variables are used to store each section of the user's input.
            
            String input = console.readLine("\n\n -> ");

            // The user's input is split into at most 3 separate strings
            String[] strings = input.split(" ", 3);

            // Checks that the user correctly utilized one of the commands. If their input was split into less than 3 strings,
            // the input is rejected and the user is asked to try again.
            if(strings.length != 3) {
                System.out.println("\nClient-Side Error: Invalid command. Please use 'login' followed by your username and password,");
                System.out.println("or 'newuser' followed by your desired username and password.");

            } else {
            
                // Variables receive value.
                command = strings[0];
                username = strings[1];
                password = strings[2];

                // Checks correct usage and spelling of the commands 'login' and 'newuser', and sends an error message if they were used incorrectly.
                if(command.equals("login") == false && command.equals("newuser") == false) {
                    System.out.println("\nClient-Side Error: Invalid command. Please use 'login' followed by your username and password,");
                    System.out.println("or 'newuser' followed by your desired username and password.");
                
                
                } else if( command.equals("newuser")) {

                    if(username.length() < 3 || username.length() > 32 || password.length() < 4 || password.length() > 8) {
                        System.out.println("\nClient-Side Error: Username or password has an invalid length. Please try again.");
                    } else {
                        validRequest = true;
                    }
                } else if( command.equals("login")) {
                        validRequest = true;
                }
            }
        }

        out.println(command + " " + username + " " + password);
        return username;
    }

    // This function allows the Client to send messages to the Server and logout of the Server.
    // @param Console console - The current instance of the console is passed in for ease of access.
    // @param String username - The username of the currently logged in user.
    void messager(Console console, String username) {

        System.out.println("\nTo send a message to the server, type 'send', followed by a space, and your desired message.");
        boolean validMessage = false;
        
        // This loop is used to ensure that the user enters an acceptable message.
        while(!validMessage) {
            String input;
            String command;
            String message;

            input = console.readLine(" -> ");

            // User's input is divided into two strings, the command portion and the message portion.
            String[] strings = input.split(" ", 2);
            command = strings[0];

            // Checks for correct usage of the 'logout' command and executes if true.
            if(command.equals("logout") && input.equals("logout")) {
                System.out.println("\nLogging you out... Come back soon!");
                logout();
                break;

            // Checks for correct usage of the 'send' command and executes if true.
            } else if(command.equals("send")) {

                message = strings[1];

                // Ensures that the message meets length requirements.
                if(message.length() > 256) {
                    System.out.println("\nClient-Side Error: Invalid Message, messages cannot exceed 256 characters.");
                } else {
                    validMessage = true;
                    System.out.println("\nMessage is valid! Sending message to server...");
                    out.println(input);
                }

            // Notifies the user that their input was invalid.
            } else {
                System.out.println("\nClient-Side Error: Invalid Message, please type 'send', followed by a space, and your desired message,");
                System.out.println("\nor type 'logout' to disconnect.");
            }
        }
    }

    // This function closes the socket instance, terminating the Client.
    void logout() {
        try {
            socket.close();
        } catch (IOException ex) {
 
            System.out.println("Error writing to server: " + ex.getMessage());
        }
    }
}