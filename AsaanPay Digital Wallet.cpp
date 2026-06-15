#include <iostream>   // For input/output operations like cin, cout
#include <string>     // For using string data type
#include <cstdio>     // For sprintf function
#include <cctype>     // For character functions like isdigit(), tolower()
#include <cstdlib>    // For system("cls") and system("pause")
#include <sstream>    // For stringstream to safely parse strings to numbers
using namespace std;

//Basic Utilite Functions

//Converts Double To String to easily add amount to history
//Because history is an string
string toStr(double x)
{
    char buf[50];
    sprintf(buf, "%.2f", x);
    return string(buf);
}

//Converts String to lower case for case-insensitive
string lowerStr(string s)
{
    for (int i = 0; i < (int)s.length(); i++)
        s[i] = tolower(s[i]);
    return s;
}

// Generates a unique account ID like AP1001, AP1002...
string generateID(int index)
{
    char buf[20];
    sprintf(buf, "AP%04d", index + 1001);
    return string(buf);
}

// Checks PIN to be exactly 4 digits
bool isValidPin(const string &p)
{
    if (p.length() != 4) return false;
    for (int i = 0; i < (int)p.length(); i++)
        if (!isdigit(p[i])) return false;
    return true;
}

// Checks Email to be only Lower case
bool isEmailLower(const string &em)
{
    for (int i = 0; i < (int)em.length(); i++)
        if (isupper(em[i])) return false;
    return true;
}

//  USER CLASS
class User
{
	// Encapsulation: sensitive user data like pin account ID is kept private
	private:
		
    string name, pin, email, accountID;
    double balance;
    string history[200];
    int historyCount;

    // Static Variable (across the system)
    static int totalUsers;
    static int totalTransactions;
    static double totalSystemBalance;

	public:
		
 	//Construstors		
    User()
	{
        name = ""; pin = "0000"; email = "";
        accountID = ""; balance = 0; historyCount = 0;
    }

    User(string n, string p, string em, string id, double bal = 0)
	{
        name = n; pin = p; email = em;
        accountID = id; balance = bal;
        historyCount = 0;
    }

    // Static Control
    static void addUser(double bal)
	{
        totalUsers++;
        totalSystemBalance += bal;
    }

    static void addTransaction() { totalTransactions++; }
    static int getTotalUsers() { return totalUsers; }
    static int getTotalTransactions() { return totalTransactions; }
    static double getTotalSystemBalance() { return totalSystemBalance; }

	// Getters & Setter: gets and sets values of the member through functions 
    string getName()      { return name; }
    string getPin()       { return pin; }
    string getEmail()     { return email; }
    string getAccountID() { return accountID; }
    double getBalance()   { return balance; }
    void   setPin(string p) { pin = p; }

  	// History Management 
    // Function overloading: two methods to add history entries with or without note

	//without a note only balance 
    void addHistory(string entry)
	{
        if (historyCount < 200)
            history[historyCount++] = entry;
    }

	//with a note
    void addHistory(string entry, string note)
	{
        if (historyCount < 200)
            history[historyCount++] = entry + " | Note: " + note;
    }

    // Wallet Operations
    // Deposit money
    bool deposit(double amt)
	{
        if (amt <= 0) return false;
        balance += amt;
        totalSystemBalance += amt;
        addTransaction();
        addHistory("Deposited Rs." + toStr(amt));
        return true;
    }

    // Withdraw money
    bool withdraw(double amt)
	{
        if (amt <= 0 || amt > balance) return false;
        balance -= amt;
        totalSystemBalance -= amt;
        addTransaction();
        addHistory("Withdrew Rs." + toStr(amt));
        return true;
    }

    // Withdraw money from wallet
    //without a note
    bool sendMoney(User &recv, double amt)
	{
        if (amt <= 0 || amt > 50000 || amt > balance) return false;

        balance -= amt;
        recv.balance += amt;
        addTransaction();

        addHistory("Sent Rs." + toStr(amt) +
                   " to " + recv.name + " (" + recv.accountID + ")");

        recv.addHistory("Received Rs." + toStr(amt) +
                        " from " + name + " (" + accountID + ")");

        return true;
    }

	//Overloaded with a note
    bool sendMoney(User &recv, double amt, string note)
	{
        if (amt <= 0 || amt > 50000 || amt > balance) return false;

        balance -= amt;
        recv.balance += amt;
        addTransaction();

        addHistory("Sent Rs." + toStr(amt) +
                   " to " + recv.name + " (" + recv.accountID + ")", note);

        recv.addHistory("Received Rs." + toStr(amt) +
                        " from " + name + " (" + accountID + ")", note);

        return true;
    }

    //Display History 
    void showHistory()
	{
        system("cls");
        cout << "========================================\n";
        cout << "      AsaanPay - Transaction History    \n";
        cout << "========================================\n";
        cout << "  " << name << " | " << accountID << "\n";
        cout << "========================================\n";

        if (historyCount == 0)
            cout << "  No transactions yet.\n";
        else
            for (int i = 0; i < historyCount; i++)
                cout << "  " << i + 1 << ". " << history[i] << "\n";

        cout << "========================================\n";
        system("pause");
    }
};

// Static variables Declaration (Done Outside The class always)
int User::totalUsers = 0;
int User::totalTransactions = 0;
double User::totalSystemBalance = 0;


//  Search Users Function
int searchUsers(User users[], int count, string term, int excludeIdx)
{
    string s = lowerStr(term);
    int idx[100], found = 0;

    for (int i = 0; i < count; i++)
	{
        if (i == excludeIdx) continue;

        string n = lowerStr(users[i].getName());
        string id = lowerStr(users[i].getAccountID());
        string em = lowerStr(users[i].getEmail());

        if (n.find(s) != string::npos ||
            id.find(s) != string::npos ||
            em.find(s) != string::npos)
            idx[found++] = i;
    }

    if (found == 0)
	{
        cout << "========================================\n";
        cout << "  No matching users found.\n";
        cout << "========================================\n";
        system("pause");
        return -1;
    }

    cout << "========================================\n";
    cout << "           SEARCH RESULTS\n";
    cout << "========================================\n";
    for (int i = 0; i < found; i++)
	{
        cout << "  " << i + 1 << ". " << users[idx[i]].getName() << "\n";
        cout << "     ID    : " << users[idx[i]].getAccountID() << "\n";
        cout << "     Email : " << users[idx[i]].getEmail() << "\n";
    }
    cout << "  0. Cancel\n";
    cout << "========================================\n";
    cout << "  Select: ";

    string input;
    int choice;
    cin >> input;
    stringstream ss(input);
    if (!(ss >> choice) || !(ss.eof()) || choice < 0 || choice > found)
        return -1;

    if (choice == 0) return -1;
    return idx[choice - 1];
}

//  Main Program
int main()
{
    User users[100];
    int userCount = 0;
    int current = -1;

    //Initial Users
    users[0] = User("Sir Hassan Mujtaba", "1234", "hassanmujtaba@gmail.com", generateID(0), 20000);
    User::addUser(20000);

    users[1] = User("Badar Dhindsa", "8238", "badardhindsa@gmail.com", generateID(1), 5000);
    User::addUser(5000);

    users[2] = User("Fahad Malik", "8240", "fahadmalik@gmail.com", generateID(2), 5000);
    User::addUser(5000);

    users[3] = User("Talha Maqsood", "8385", "talhamaqsood@gmail.com", generateID(3), 5000);
    User::addUser(5000);

    users[4] = User("Hanzla", "6767", "hanzla@gmail.com", generateID(4), 5000);
    User::addUser(5000);

    userCount = 5;

    //loop: handles both main menu & user menu
    while (true)
	{
        system("cls");

        if (current == -1)
		{
			// MAIN MENU
            cout << "========================================\n";
            cout << "     AsaanPay Digital Wallet\n";
            cout << "========================================\n";
            cout << "  1. Login\n";
            cout << "  2. Create Wallet\n";
            cout << "  3. System Info\n";
            cout << "  0. Exit\n";
            cout << "========================================\n";
            cout << "  Choice: ";

            string input;
            int opt;
            cin >> input;
            stringstream ss(input);
            if (!(ss >> opt) || !(ss.eof()))
			{
                cout << "Invalid option.\n";
                system("pause");
                continue;
            }

            switch (opt)
			{
            case 0:
                system("cls");
                cout << "========================================\n";
                cout << "   Thank you for using AsaanPay!\n";
                cout << "========================================\n";
                return 0;

            case 1:
			{
                system("cls");
                cout << "========================================\n";
                cout << "              LOGIN\n";
                cout << "========================================\n";
                cout << "  (Enter 0 as name to cancel)\n";
                cout << "========================================\n";

                string n, p;
                cin.ignore();
                cout << "  Name : ";
                getline(cin, n);
                if (n == "0") break;

                cout << "  PIN  : ";
                cin >> p;
                if (p == "0") break;

                int found = -1;
                for (int i = 0; i < userCount; i++)
                    if (users[i].getName() == n && users[i].getPin() == p)
                        found = i;

                cout << "========================================\n";
                if (found == -1)
                    cout << "  Invalid name or PIN.\n";
                else
				{
                    current = found;
                    cout << "  Login Successful!\n";
                    cout << "  Welcome, " << users[current].getName() << "!\n";
                }
                cout << "========================================\n";
                system("pause");
                break;
            }

            case 2:
			{
                system("cls");
                cout << "========================================\n";
                cout << "        CREATE NEW WALLET\n";
                cout << "========================================\n";
                cout << "  (Enter 0 at any field to cancel)\n";
                cout << "========================================\n";

                string n, em, p;

                cin.ignore();
                cout << "  Name  : ";
                getline(cin, n);
                if (n == "0") break;
                while (n.empty())
				{
                    cout << "  Name cannot be blank (0 to cancel): ";
                    getline(cin, n);
                    if (n == "0") break;
                }
                if (n == "0") break;

                cout << "  Email : ";
                getline(cin, em);
                if (em == "0") break;

                while (!isEmailLower(em))
				{
                    cout << "  Email cannot contain uppercase letters!\n";
                    cout << "  Enter again (0 to cancel): ";
                    getline(cin, em);
                    if (em == "0") break;
                }
                if (em == "0") break;

                cout << "  PIN   : ";
                cin >> p;
                if (p == "0") break;
                while (!isValidPin(p))
				{
                    cout << "  Must be exactly 4 digits (0 to cancel): ";
                    cin >> p;
                    if (p == "0") break;
                }
                if (p == "0") break;

                string id = generateID(userCount);
                users[userCount++] = User(n, p, em, id);
                User::addUser(0);

                cout << "========================================\n";
                cout << "  Wallet Created Successfully!\n";
                cout << "  Name       : " << n << "\n";
                cout << "  Account ID : " << id << "\n";
                cout << "  Email      : " << em << "\n";
                cout << "========================================\n";
                system("pause");
                break;
            }

            case 3:
			{
                system("cls");
                cout << "========================================\n";
                cout << "          SYSTEM INFORMATION\n";
                cout << "========================================\n";
                cout << "  Total Users        : " << User::getTotalUsers() << "\n";
                cout << "  Total Transactions : " << User::getTotalTransactions() << "\n";
                cout << "  Total Balance      : Rs. " << toStr(User::getTotalSystemBalance()) << "\n";
                cout << "========================================\n";
                system("pause");
                break;
            }

            default:
                cout << "Invalid option.\n";
                system("pause");
            }
        }

        else
		{ 
			// USER MENU
            User &u = users[current];

            cout << "========================================\n";
            cout << "     AsaanPay Digital Wallet\n";
            cout << "========================================\n";
            cout << "  User    : " << u.getName() << "\n";
            cout << "  ID      : " << u.getAccountID() << "\n";
            cout << "  Balance : Rs. " << toStr(u.getBalance()) << "\n";
            cout << "========================================\n";
            cout << "  1. Top-Up\n";
            cout << "  2. Cash-Out\n";
            cout << "  3. Send Money\n";
            cout << "  4. Transaction History\n";
            cout << "  5. Change PIN\n";
            cout << "  0. Logout\n";
            cout << "========================================\n";
            cout << "  Choice: ";

            string input;
            int opt;
            cin >> input;
            stringstream ss(input);
            if (!(ss >> opt) || !(ss.eof()))
			{
                cout << "Invalid option.\n";
                system("pause");
                continue;
            }

            switch (opt)
			{
            case 0:
                system("cls");
                cout << "========================================\n";
                cout << "  Goodbye, " << u.getName() << "!\n";
                cout << "  You have been logged out.\n";
                cout << "========================================\n";
                system("pause");
                current = -1;
                break;

            case 1:
			{ 
				// Top-Up
                system("cls");
                cout << "========================================\n";
                cout << "              TOP-UP\n";
                cout << "========================================\n";
                cout << "  Amount (0 to cancel): ";
                string amtStr;
                double amt;
                cin >> amtStr;
                stringstream ssAmt(amtStr);
                if (!(ssAmt >> amt) || !(ssAmt.eof())) { cout << "Invalid amount.\n"; system("pause"); break; }

                cout << "========================================\n";
                if (amt == 0)
                    cout << "  Cancelled.\n";
                else if (!u.deposit(amt))
                    cout << "  Invalid amount.\n";
                else
				{
                    cout << "  Top-Up Successful!\n";
                    cout << "  New Balance: Rs. " << toStr(u.getBalance()) << "\n";
                }
                cout << "========================================\n";
                system("pause");
                break;
            }

            case 2:
			{ 
				// Cash-Out
                system("cls");
                cout << "========================================\n";
                cout << "             CASH-OUT\n";
                cout << "========================================\n";
                cout << "  Amount (0 to cancel): ";
                string amtStr;
                double amt;
                cin >> amtStr;
                stringstream ssAmt(amtStr);
                if (!(ssAmt >> amt) || !(ssAmt.eof())) { cout << "Invalid amount.\n"; system("pause"); break; }

                cout << "========================================\n";
                if (amt == 0)
                    cout << "  Cancelled.\n";
                else if (!u.withdraw(amt))
                    cout << "  Insufficient balance or invalid amount.\n";
                else
				{
                    cout << "  Cash-Out Successful!\n";
                    cout << "  New Balance: Rs. " << toStr(u.getBalance()) << "\n";
                }
                cout << "========================================\n";
                system("pause");
                break;
            }

            case 3:
			{ 
				// Send Money
                system("cls");
                cout << "========================================\n";
                cout << "            SEND MONEY\n";
                cout << "========================================\n";
                cout << "  Search: ";

                cin.ignore();
                string s;
                getline(cin, s);

                if (s == "0") break;

                int idx = searchUsers(users, userCount, s, current);
                if (idx == -1) break;

                User &recv = users[idx];

                system("cls");
                cout << "========================================\n";
                cout << "            SEND MONEY\n";
                cout << "========================================\n";
                cout << "  To     : " << recv.getName() << "\n";
                cout << "  ID     : " << recv.getAccountID() << "\n";
                cout << "========================================\n";
                cout << "  Amount (0 to cancel): ";

                string amtStr;
                double amt;
                cin >> amtStr;
                stringstream ssAmt(amtStr);
                if (!(ssAmt >> amt) || !(ssAmt.eof())) { cout << "Invalid amount.\n"; system("pause"); break; }

                if (amt == 0) { cout << "Cancelled.\n"; system("pause"); break; }

                cout << "  Note (Enter to skip): ";
                cin.ignore();
                string note;
                getline(cin, note);

                cout << "========================================\n";
                cout << "  To     : " << recv.getName() << " (" << recv.getAccountID() << ")\n";
                cout << "  Amount : Rs. " << toStr(amt) << "\n";
                if (note != "")
                    cout << "  Note   : " << note << "\n";

                cout << "========================================\n";
                cout << "  Confirm? (y/n): ";
                char c; cin >> c;

                if (c != 'y' && c != 'Y')
                    cout << "  Transaction Cancelled.\n";
                else
				{
                    bool ok;
                    if (note == "")
                        ok = u.sendMoney(recv, amt);
                    else
                        ok = u.sendMoney(recv, amt, note);

                    if (!ok)
                        cout << "  Failed. Check amount or balance.\n";
                    else
                        cout << "  Money Sent Successfully!\n"
                             << "  New Balance: Rs. " << toStr(u.getBalance()) << "\n";
                }
                cout << "========================================\n";
                system("pause");
                break;
            }

            case 4: u.showHistory(); break;
			case 5:
			{
				// Change PIN
				system("cls");
				cout << "========================================\n";
				cout << "            CHANGE PIN\n";
				cout << "========================================\n";

				string oldPin;
				cout << "  Enter OLD PIN (0 to cancel): ";
				cin >> oldPin;

				if (oldPin == "0")
				{
					cout << "  Cancelled.\n";
					system("pause");
					break;
				}

				// Verify old PIN
				if (oldPin != u.getPin())
				{
					cout << "========================================\n";
					cout << "  Incorrect old PIN!\n";
					cout << "========================================\n";
					system("pause");
					break;
				}

				// Ask for new PIN
				string newPin, confirmPin;

				cout << "  Enter NEW 4-digit PIN (0 to cancel): ";
				cin >> newPin;

				if (newPin == "0")
				{
					cout << "  Cancelled.\n";
					system("pause");
					break;
				}

				while (!isValidPin(newPin))
				{
					cout << "  Invalid PIN. Must be 4 digits (0 to cancel): ";
					cin >> newPin;

					if (newPin == "0")
					{
						cout << "  Cancelled.\n";
						system("pause");
						break;
					}
				}

				if (newPin == "0") break;

				// Confirm new PIN
				cout << "  Confirm NEW PIN: ";
				cin >> confirmPin;

				if (confirmPin != newPin)
				{
					cout << "========================================\n";
					cout << "  PINs do NOT match! PIN not changed.\n";
					cout << "========================================\n";
					system("pause");
					break;
				}

				// Change PIN
				u.setPin(newPin);
				cout << "  PIN Changed Successfully!\n";
				cout << "========================================\n";
				system("pause");
				break;
			}
            default:
                cout << "Invalid option.\n";
                system("pause");
            }
        }
    }

    return 0;
}
