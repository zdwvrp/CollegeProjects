import java.io.*;
import java.net.*;
import java.util.*;

public class Server {
    private final int port = 17050;
    private Set<String> users = new HashSet<>();
    private Set<UserThread> userThreads = new HashSet<>();
    private HashMap<String, String> userInfo = new HashMap<String, String>();
    

    public static void main( String[] args ) {

        // Ensures Server is started without any additional arguments.
        if( args.length > 0 ) {
            System.out.println("\nError: No args needed to run.\n");
            System.exit(0);
        }

        // Server starts.
        Server server = new Server();
        server.retrieveUserData();
        server.start();
    }

    // Retrieve usernames and passwords from users.txt.
    void retrieveUserData() {
        try {
            File userInfoFile = new File("users.txt");
            Scanner in = new Scanner(userInfoFile);

            while( in.hasNextLine() ) {

                // Parentheses and space inbetween comma and password are removed from user records when retrieving them.
                String user = in.nextLine();
                user = user.replace("(", "");
                user = user.replace(")", "");
                user = user.replace(" ", "");

                String[] usernamePassword = user.split(",");
                userInfo.put(usernamePassword[0], usernamePassword[1]);
            }
            in.close();

        } catch (FileNotFoundException e) {
            System.out.println("Error: Couldn't retrieve user login information\n");
            e.printStackTrace();
        }
    }

    // Method for starting an instance of the Server.
    void start() {

        // The socket is initialized and a new UserThread is created that awaits a Client connection.
        try( ServerSocket socket = new ServerSocket(port) ) {
            while (true) {
                Socket serverAccept = socket.accept();
                UserThread newUser = new UserThread(serverAccept, this);
                userThreads.add(newUser);
                newUser.start();
            }

        } catch ( IOException e ) {
            System.out.println( "\nServer Error: " + e.getMessage() + "\n" );
            e.printStackTrace();
        }
    }

    // Sends a message to all current users.
    // @param String msg - Message to be sent to users.
    void sendMessage( String msg ) {

        if( msg.length() > 256 ) {

            System.out.println( "\nError: Message exceeds maximum length (256 characters)\n" );
        } else {

            for( UserThread user : userThreads ) {
                user.sendMessage( msg );
            }
        }
    }

    // Adds a new user to the Set of current users.
    void addUser( String username ) {
        users.add(username);
    }

    // Removes a user from the Set of current users.
    void removeUser( String username, UserThread user ) {
        boolean removed = users.remove(username);
        if( removed == true ) {
            userThreads.remove( user );
            System.out.println( "\nUser " + username + " has disconnected\n" );
        }
    }

    // @return HashMap<String, String> - Returns the Map containing all user accounts that exist on the server.
    HashMap<String, String> getUserInfo() {
        return userInfo;
    }

    // Add a new user to the HashMap userInfo and append the user's credentials in the 'users.txt' file.
    // @param String username - The username of the user to be added into records.
    // @param String password - The password of the user to be added into records.
    void addNewUser(String username, String password) {
        
        userInfo.put(username, password);
        try {
            FileWriter writer = new FileWriter("users.txt", true);
            writer.write("\n(" + username + ", " + password + ")");
            writer.close();
        } catch (IOException e) {
            System.out.println("\nError: Couldn't write to users.txt");
            e.printStackTrace();
        }
    }
}