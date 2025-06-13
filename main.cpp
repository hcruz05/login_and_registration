/*
 *      Title: Login and Registration
 *      Developers: Marcelo Orellana, Roderick Lee, Heriberto Santos, Mauricio Guaido
 *      Class: COP2006 Programming 1
 *      Instructor: Dr. Deepa Devasenapathy
 *      About: C++ Program that allows user to create accounts using their desired username and password. Allows user to change passwords or delete their account.
 */

// Libraries
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// class User class for username and password
class User
{
public:
    string username;
    string password;

    User(string u, string p)
    {
        username = u;
        password = p;
    }
};

// maximum users allowed is 100
const int MAX_USERS = 100;
User* users[MAX_USERS];
int userCount = 0;


// Loads users from users.txt to users array
void loadUsers()
{
    ifstream file("users.txt");
    string line;

    // initial count is 0
    userCount = 0;

    while (getline(file, line))
    {
        // skips over empty lines
        if (line.empty())
        {
            continue;
        }

        // skips the leading spaces
        size_t spacePos = line.find(' ');
        if (spacePos == string::npos)
        {
            continue;
        }

        string username = line.substr(0, spacePos);
        string password = line.substr(spacePos + 1);

        if (userCount < MAX_USERS)
        {
            users[userCount++] = new User(username, password);
        }
    }

    file.close();
}

 // Saves users to users.txt
void saveUsers()
{
    ofstream file("users.txt", ios::trunc);

    for (int i = 0; i < userCount; i++)
    {
        file << users[i]->username << " " << users[i]->password << endl;
    }

    file.close();
}

// Checks if username exists already
int findUserIndex(const string& username)
{
    for (int i = 0; i < userCount; i++)
    {
        if (users[i]->username == username)
        {
            return i;
        }
    }
    return -1;
}

// Removes input buffers
void removeInputBuffer()
{
    char c;
    while (cin.get(c) && c != '\n');
}

// Register new user with a username (no spaces) and password (spaces allowed)
void registerUser()
{
    string username;
    string password;

    cout << "\n=== Register ===\n";
    cout << "Note: Username cannot contain spaces.\n";

    while (true)
    {
        cout << "Enter a username: ";
        getline(cin, username);

        if (username.find(' ') != string::npos)
        {
            cout << "Error: Username cannot contain spaces.\n";
        }
        else if (username.empty())
        {
            cout << "Error: Username cannot be empty.\n";
        }
        else if (findUserIndex(username) != -1)
        {
            cout << "Error: Username already exists.\n";
        }
        else
        {
            break;
        }
    }

    cout << "Enter a password (spaces allowed): ";
    getline(cin, password);

    if (userCount < MAX_USERS)
    {
        users[userCount++] = new User(username, password);
        saveUsers();
        cout << "Registration successful!\n";
    }
    else
    {
        cout << "User limit reached. Cannot register more users.\n";
    }
}

// Login by checking username and password
int loginUser()
{
    string username;
    string password;

    cout << "\n=== Login ===\n";
    cout << "Enter username: ";
    getline(cin, username);

    // Checks if username exists in file
    int index = findUserIndex(username);

    if (index == -1)
    {
        cout << "Error: Username does not exist.\n";
        return -1;
    }

    cout << "Enter password: ";
    getline(cin, password);

    if (users[index]->password == password)
    {
        cout << "Login successful!\n";
        return index;
    }
    else
    {
        cout << "Incorrect password.\n";
        return -1;
    }
}

// Allows user to change password
void changePassword(int index)
{
    string newPassword;

    cout << "\n=== Change Password ===\n";
    cout << "Enter new password: ";

    getline(cin, newPassword);

    users[index]->password = newPassword;
    saveUsers();
    cout << "Password changed successfully.\n";
}

// Deletes currently logged-in user
void deleteUser(int index)
{
    cout << "\n=== Delete Account ===\n";
    cout << "Account " << users[index]->username << " deleted.\n";

    delete users[index];

    for (int i = index; i < userCount - 1; i++)
    {
        users[i] = users[i + 1];
    }

    userCount--;
    saveUsers();
}

// function to delete all users created when prompted in main function, otherwise this does nothing
void deleteAllUsers()
{
    for (int i = 0; i < userCount; i++)
    {
        delete users[i];
    }
    userCount = 0;
    saveUsers(); // saves an empty file
    cout << "All user accounts deleted.\n";
}


/*
 * Main program. Allows user to choose to register, login, or exit
 * When successfully logged in, user can choose to change password, delete account, or log out
*/
int main()
{
    loadUsers();
    int choice;

    while (true)
    {
        cout << "\n=== Main Menu ===\n";
        cout << "1. Register\n";
        cout << "2. Login\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        removeInputBuffer();

        switch (choice)
        {
            case 1:
            {
                registerUser();
                break;
            }
            case 2:
            {
                // User can change passowrd, delete account, or logout to main menu
                int index = loginUser();
                if (index != -1)
                {
                    int subChoice;
                    do
                    {
                        cout << "\n=== Account Menu ===\n";
                        cout << "1. Change Password\n";
                        cout << "2. Delete Account\n";
                        cout << "3. Logout\n";
                        cout << "Enter your choice: ";
                        cin >> subChoice;
                        removeInputBuffer();

                        switch (subChoice)
                        {
                            case 1:
                            {
                                changePassword(index);
                                break;
                            }
                            case 2:
                            {
                                deleteUser(index);
                                subChoice = 3; // logouts after deletion
                                break;
                            }
                            case 3:
                            {
                                cout << "Logged out.\n";
                                break;
                            }
                            default:
                            {
                                cout << "Invalid option.\n";
                            }
                        }
                    } while (subChoice != 3);
                }
                break;
            }
            case 3:
            {
                cout << "Program complete!\n";
                // Deletes the memory created and closes program
                for (int i = 0; i < userCount; i++)
                {
                    delete users[i];
                }
                return 0;
            }
            // Error if choice chosen is invalid
            default:
            {
                cout << "Invalid option.\n";
            }
        }
    }

    return 0;
}
