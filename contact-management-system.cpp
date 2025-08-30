#include <iostream>
#include <fstream>
#include <string>
using namespace std;
//user struct
struct User
{
    string username;
    string password;
};
//contact struct
struct Contact
{
    string firstName;
    string lastName;
    string phone;
};
// Validation helpers
bool ensureGroupExists(User& u, const string& gname);
bool ensureContactNotInGroup(User& u, const string& gname, const Contact& c);

// Core contact handling
void appendContactToGroup(User& u, const string& gname, const Contact& c);
bool addContactToGroup(User& u, const string& gname, const Contact& c);
bool removeContactFromGroup(User& u, const string& gname, const Contact& c);

// Group/Contact helpers
bool groupExists(User& u, const string& gname);
bool isContactInGroup(User& u, const string& gname, const Contact& c);
bool sameContact(const Contact& a, const Contact& b);
string getGroupsFileName(User& u);

// Read/write helpers
bool readGroupMember(ifstream& fin, Contact& c);
void writeContactToFile(ofstream& fout, const Contact& c);

// View 
bool viewGroups(User& u);
bool viewGroupsWithMembers(User& u);
void printGroupMembers(ifstream& fin);
void skipGroupMembers(ifstream& fin);

// Messaging
void sendMessageToGroup(User& u, const string& gname);
void sendMessageToMembers(ifstream& fin);

bool takeInput(User& user)
{
    cout << "Entre your name please: \n";
    getline(cin, user.username);
    cout << "Entre your password please: \n";
    getline(cin, user.password);


    if (user.username.empty() || user.password.empty())
    {
        return false;
    }
    return true;
}

bool checkFileOpen(ifstream& fin, const string& filename)
{
    return fin.is_open();

}

bool checkFileOpen(ofstream& fout, const string& filename)
{
    return fout.is_open();

}

bool signUp(User& user)
{
    
    ofstream fout;
    fout.open("users.txt", ios::app);
    if (!checkFileOpen(fout, "users.txt"))
    {
        return false;
    }
    fout << user.username << '\n' << user.password << '\n';

    fout.close();
    return true;

}
bool loginUser(User& user)
{ 

    ifstream fin;
    fin.open("users.txt");
    if (!checkFileOpen(fin, "users.txt"))
    {
        return false;
    }
    string username, password;

    while (getline(fin, username) && getline(fin, password))
    {

        if (username == user.username && password == user.password)
        {
            return true;
        }
    }
    return false;
}
void printMenu()
{
    cout << "\n--- Menu ---\n";
    cout << "----You have logged in successfully, now select option what do you wnat to do now?----\n\n";
    cout << " 1. View All Contacts (Sorted)\n";
    cout << " 2. Add Contact(Check uniqueness, overwrite if confirmed)\n";
    cout << " 3. Search Contact(Partial name match)\n";
    cout << " 4. Update Contact Number\n";
    cout << " 5. Delete Contact\n";
    cout << " 6. Create Group\n";
    cout << " 7. View Groups\n";
    cout << " 8. Add / Remove Contact from Group\n";
    cout << " 9. Send Message to Contact or Group\n";
    cout << " 10. Logout\n";
}
void checkLoginAndPrintMenu(User& user)
{
    if (loginUser(user)) 
    {
        printMenu();
    }
   
}
bool isValidPhone(const string& phone)
{
    for (int i = 0; i < (int)phone.size(); i++)
    {
        if (!isdigit(phone[i]))
        {
            return false;
        }
    }
    return true;
}
Contact takeContactInput()
{
    Contact contact;
    cout << "Entre the first name of contact:\n";
    getline(cin, contact.firstName);
    cout << "Entre the last name of contact:\n";
    getline(cin, contact.lastName);

    cout << "Entre the contact number of person :\n";
    getline(cin, contact.phone);
    while (!isValidPhone(contact.phone))
    {
        cout << "Entre the VALID contact number of person:\n";
        getline(cin, contact.phone);
    }
    return contact;
}
string getFileName(User& user)
{
    return "File_" + user.username + ".txt";

}
bool contactFileExists(User& user)
{
    ifstream fin(getFileName(user));
    return fin.is_open();
}

bool createFile(User& user)
{
    if (!contactFileExists(user))
    {
        ofstream fout(getFileName(user));
        fout.close();
        return true;
    }
    return false;
}

string toLowerCase(const string& str)
{
    string result = str;
    for (int i = 0; i < result.length(); i++)
    {
        if (result[i] >= 'A' && result[i] <= 'Z')
        {
            result[i] = result[i] - 'A' + 'a';
        }
    }
    return result;
}
int findDuplicateContact(Contact* contacts, int count, const Contact& newContact)
{

    for (int i = 0; i < count; i++)
    {
        if ((toLowerCase(contacts[i].firstName) == toLowerCase(newContact.firstName) && toLowerCase(contacts[i].lastName) == toLowerCase(newContact.lastName)) && (contacts[i].phone == newContact.phone))
        {
            return i;
        }
    }
    return -1;
}

int findByName(Contact* contacts, int count, const Contact& newContact)
{
    for (int i = 0; i < count; i++)
    {
        if (toLowerCase(contacts[i].firstName) == toLowerCase(newContact.firstName) && toLowerCase(contacts[i].lastName) == toLowerCase(newContact.lastName))
        {
            return i;
        }
    }
    return -1;
}

int findByPhone(Contact* contacts, int count, const string& phone)
{
    for (int i = 0; i < count; i++)
    {
        if (contacts[i].phone == phone)
        {
            return i;
        }
    }
    return -1;
}

bool askOverwriteConfirmation(const Contact& existing)
{
    cout << "A contact already exists:\n";
    cout << existing.firstName << " " << existing.lastName << " - " << existing.phone << "\n";

    cout << "Do you want to overwrite it? (y/n): ";
    char choice;
    cin >> choice;
    cin.ignore();
    return (choice == 'y' || choice == 'Y');
}

void saveAllContactsToFile(User& user, Contact* contacts, int count)
{
    ofstream fout(getFileName(user), ios::trunc);
    if (!checkFileOpen(fout, getFileName(user)))
    {
        return;
    }
    for (int i = 0; i < count; i++)
    {
        fout << contacts[i].firstName << '\n';
        fout << contacts[i].lastName << '\n';
        fout << contacts[i].phone << '\n';
    }

    fout.close();
}


bool overwriteContact(User& user, Contact* contacts, int count, int index, const Contact& newContact)
{
    contacts[index] = newContact;
    saveAllContactsToFile(user, contacts, count);
    return contactFileExists(user);
}


Contact* loadContacts(User& user, int& count)
{
    ifstream fin(getFileName(user));
    if (!checkFileOpen(fin, getFileName(user)))
    {
        return nullptr;
    }
    string input;
    while (getline(fin, input))
    {
        count = count + 1;
    }
    fin.close(); 
    if (count == 0)
    {
        return nullptr;
    }
    if (count % 3 != 0)
    {
        count = 0;
        return nullptr;
    }
    count = count / 3;
    Contact* contact = new Contact[count];
    fin.open(getFileName(user));
    for (int i = 0; i < count; i++)
    {
        getline(fin, contact[i].firstName);
        getline(fin, contact[i].lastName);
        getline(fin, contact[i].phone);
    }
    fin.close();
    return contact;
}


bool checkDuplicateAndHandle(User& user, Contact*& contacts, int& count, const Contact& newContact)
{
    int duplicateIndex = findDuplicateContact(contacts, count, newContact);
    if (duplicateIndex != -1)
    {
        if (askOverwriteConfirmation(contacts[duplicateIndex]))
        {
            overwriteContact(user, contacts, count, duplicateIndex, newContact);
            return true;
        }
        return true;
    }

    int nameDuplicate = findByName(contacts, count, newContact);
    if (nameDuplicate != -1)
    {
        cout << "A contact with the same name already exists:\n";
        cout << contacts[nameDuplicate].firstName << " " << contacts[nameDuplicate].lastName << " - " << contacts[nameDuplicate].phone << "\n";

        if (askOverwriteConfirmation(contacts[nameDuplicate]))
        {
            overwriteContact(user, contacts, count, nameDuplicate, newContact);
            return true;
        }
        return true;
    }

    int phoneDuplicate = findByPhone(contacts, count, newContact.phone);
    if (phoneDuplicate != -1)
    {
        cout << "This phone number is already assigned to another contact:\n";
        cout << contacts[phoneDuplicate].firstName << " " << contacts[phoneDuplicate].lastName << " - " << contacts[phoneDuplicate].phone << "\n";

        if (askOverwriteConfirmation(contacts[phoneDuplicate]))
        {
            overwriteContact(user, contacts, count, phoneDuplicate, newContact);
            return true;
        }
        return true;
    }
    return false;

}

bool addContact(User& user)
{
    if (!contactFileExists(user))
    {
        createFile(user);
    }

    int count = 0;
    Contact* contacts = loadContacts(user, count);

    Contact newContact = takeContactInput();
    if (!contacts)
    {
        count = 0;
    }
    if (count % 3 != 0)
    {
        cout << "Corrupted contact file!\n";
        delete[] contacts;
        contacts = nullptr;
        return false;
    }


    if (checkDuplicateAndHandle(user, contacts, count, newContact))
    {
        delete[] contacts;
        contacts = nullptr;
        return false;
    }
    int newSize = (count > 0 ? count : 0) + 1;
    Contact* newArr = new Contact[count + 1];
    if (contacts && count>0)
    {
        for (int i = 0; i < count; i++)
        {
            newArr[i] = contacts[i];
        }
    }
    newArr[newSize-1] = newContact;

   
    

    saveAllContactsToFile(user, newArr, count + 1);

    delete[] newArr;
    newArr = nullptr;
    delete[] contacts;
    contacts = nullptr;
    return true;
}


void sortContacts(Contact*& contacts, int count)
{

    for (int i = 0; i < count; i++)
    {
        for (int j = 0; j < count - 1 - i; j++)
        {
            if (contacts[j].firstName > contacts[j + 1].firstName || (contacts[j].firstName == contacts[j + 1].firstName && contacts[j].lastName > contacts[j + 1].lastName))
            {
                Contact temp = contacts[j];
                contacts[j] = contacts[j + 1];
                contacts[j + 1] = temp;
            }
        }
    }
}
void displayContact(Contact*& contacts, int count)
{
    for (int i = 0; i < count; i++)
    {
        cout << contacts[i].firstName << " " << contacts[i].lastName << " " << contacts[i].phone << '\n';
    }
}
void displaySingleContact(Contact* contact,int index)
{
    cout << contact[index].firstName << " " << contact[index].lastName << " - " << contact[index].phone << "\n";

}
Contact* viewContact(User& user,int count)
{
    Contact*contacts = loadContacts(user, count);
    return contacts;
   
}

bool isSubstr(const string& str, const string& searchTerm)
{
    if (searchTerm.empty())
    {
        return false;
    }
    int i = 0;
    while (str[i] != '\0')
    {
        int j = 0;
        while (str[i + j] != '\0' && searchTerm[j] != '\0' && str[i + j] == searchTerm[j] && (i+j<(int)str.size()) )// yahan i likhny sy  search amwo out of bound gya tha
        {
            j++;
        }
        if (searchTerm[j] == '\0')
        {
            return true;
        }
        i++;
    }
    return false;
}

bool contactMatches(const Contact& contacts, const string& searchTerm)
{ // is pr kaam krna hai abhi
    string first = toLowerCase(contacts.firstName);
    string last = toLowerCase(contacts.lastName);
    string term = toLowerCase(searchTerm);

    return ((isSubstr(toLowerCase(contacts.firstName), term)) || (isSubstr(toLowerCase(contacts.lastName), term)));
}

bool searchContact(User& user, const string& searchTerm)
{
    if (searchTerm.empty())
    {
        return false;
    }
    int count = 0; 
    Contact* contact = loadContacts(user, count);
    if (contact == nullptr || count == 0)
    {
        delete[] contact;
        contact = nullptr;
        return false;
    }
    bool found = false;
    for (int i = 0; i < count; i++)
    {
        if (contactMatches(contact[i], searchTerm))
        {
            displaySingleContact(contact, i);
            found = true;
        }
    }

    delete[] contact;
    contact = nullptr;
    return found;
}
void getContactNameInput(string& firstName, string& lastName)
{
    cout << "Entre the first name of contact you want to update:\n";
    getline(cin, firstName);
    cout << "Entre the last name of contact you want to update:\n";
    getline(cin, lastName);
}

int findContactIndex(Contact* contacts, int count, const string& firstName, const string& lastName)
{
    for (int i = 0; i < count; i++)
    { //-> operator k sath index operator nhi lga skty, dot lgao
        if ((toLowerCase(firstName) == toLowerCase(contacts[i].firstName)) && (toLowerCase(lastName) == toLowerCase(contacts[i].lastName)))
        {
            return i;
        }
    }
    return -1;
}

string getValidPhoneNumber()
{ 
    string phoneNo;
    do
    {
        cout << "Entre the number of person you want to save:\n";
        getline(cin, phoneNo);
        if (!(isValidPhone(phoneNo)))
        {
            cout << "Entre a valid number\n";
        }
    } while (!(isValidPhone(phoneNo)));

    return phoneNo;
}

bool updateContactPhoneNumber(User& user)
{
    int count = 0;
    Contact* contacts = loadContacts(user, count);
    if (contacts == nullptr || count == 0)
    {
        delete[] contacts;
        contacts = nullptr;
        return false;
    }
    string firstName;
    string lastName;
    getContactNameInput(firstName, lastName);
    int index = findContactIndex(contacts, count, firstName, lastName);
    if (index == -1)
    {
        delete[] contacts;
        contacts = nullptr;

        return false;
    }
    cout << "Current phone: " << contacts[index].phone << "\n";

    string newPhone = getValidPhoneNumber();
    contacts[index].phone = newPhone;

    saveAllContactsToFile(user, contacts, count);
    delete[] contacts;
    contacts = nullptr;

    return true;
}
void removeContactAtIndex(Contact* contacts, int& count, int index)
{
    for (int i = index; i < count - 1; i++)
    {
        contacts[i] = contacts[i + 1];
    }
    count--;
}

bool deleteContact(User& user)
{
    int count = 0;
    Contact* contacts = loadContacts(user, count);
    if (contacts == nullptr || count == 0)
    {
        cout << "No contacts found to delete.\n";
        delete[] contacts;
        contacts = nullptr;
        return false;
    }

    string firstName, lastName;
    cout << "Enter the first name of the contact you want to delete:\n";
    getline(cin, firstName);
    cout << "Enter the last name of the contact you want to delete:\n";
    getline(cin, lastName);

    int index = findContactIndex(contacts, count, firstName, lastName);
    if (index == -1)
    {
        delete[] contacts;
        contacts = nullptr;
        return false;
    }

    removeContactAtIndex(contacts, count, index);
    saveAllContactsToFile(user, contacts, count);

    delete[] contacts;
    contacts = nullptr;


    return true;
}

//func to check if contact is same
bool sameContact(const Contact& a, const Contact& b)
{
    return (a.firstName == b.firstName && a.lastName == b.lastName && a.phone == b.phone);
}
// Returns the file name for storing a user's groups
string getGroupsFileName(User& user)
{
    return "groups_" + user.username + ".txt";
}
// Check if a group exists for a user
bool groupExists(User& u, const string& gname)
{
    ifstream fin(getGroupsFileName(u));
    if(!checkFileOpen(fin, getGroupsFileName(u)))
    {
        return false;
    }
    string line;
    while (getline(fin, line))
    {
        //skip empty lines
        if (line.empty())
        {
            continue;
        }
        //if target group found
        if (line == gname)
        {
            return true;
        }
        // Skip all contacts of this group
        while (getline(fin, line) && !line.empty())
        {
            continue;
        }
    }
    fin.close();
    return false;
}
// check if contact exists in current group block
bool contactExistsInGroupSection(ifstream& fin, const Contact& contact)
{
    Contact temp;
    while (getline(fin, temp.firstName))
    {
        if (temp.firstName.empty())
        {
            break;          // end of group 
        }
        // Read rest of contact details
        if (!getline(fin, temp.lastName))
        {
            break;
        }
        if (!getline(fin, temp.phone))
        {
            break;
        }
        if (sameContact(temp, contact))
        {
            return true; // contact found
        }
    }
    return false;
}
// Check if a contact exists in a specific group
bool isContactInGroup(User& user, const string& gname, const Contact& contact)
{
    ifstream fin(getGroupsFileName(user));
    if (!checkFileOpen(fin, getGroupsFileName(user)))
    {
        return false;
    }
    string line;
    while (getline(fin, line))
    {
        if (line == gname)
        {
            // Check if contact exists in this group's members
            bool found = contactExistsInGroupSection(fin, contact);
            fin.close();
            return found;
        }
        else
        {
            // skip this group block
            while (getline(fin, line) && !line.empty())
            {
                continue;
            }
        }
    }
    fin.close();
    return false;
}

// Create group without default contact
bool createGroup(User& user, const string& gname)
{
    if (groupExists(user, gname))
    {
        return false;
    }
      
    ofstream fout(getGroupsFileName(user), ios::app);
    if (!checkFileOpen(fout, getGroupsFileName(user)))
    {
        return false;
    }
    //append group name
    fout << gname << "\n\n";
    fout.close();
    return true;
}
// Group creation with default contact
bool createGroup(User& user, const string& gname, const Contact& defaultContact)
{
    if (groupExists(user, gname))
    {
        return false;
    }

    ofstream fout(getGroupsFileName(user), ios::app);
    if (!checkFileOpen(fout, getGroupsFileName(user)))
    {
        return false;
    }
    fout << gname << "\n\n";
    fout.close();
    // Add default contact 
    if (ensureContactNotInGroup(user, gname, defaultContact))
    {
        appendContactToGroup(user, gname, defaultContact);
    }
    return true;
}
// Copy all members of a group from fin to fout
bool copyGroupContacts(ifstream& fin, ofstream& fout)
{
    Contact temp;
    bool wrote = false;
    //read from fin
    while (readGroupMember(fin, temp))
    {
        //write into fout
        writeContactToFile(fout, temp);
        wrote = true;
    }
    return wrote;
}
// Copy a group block and if targetGroup matches then append new contact
void copyGroupBlock(ifstream& fin, ofstream& fout, const string& targetGroup, const Contact& newContact) {
    string line;
    while (getline(fin, line)) {
        if (line.empty()) continue; // skip empty lines
        fout << line << "\n";       // write group name

        bool isTargetGroup = (line == targetGroup);
        bool wroteMember = copyGroupContacts(fin, fout);

        if (isTargetGroup) {
            writeContactToFile(fout, newContact);
            wroteMember = true;
        }

        if (wroteMember) fout << "\n"; // ensure blank line after members
    }
}
// Append a contact to a group 
void appendContactToGroup(User& user, const string& gname, const Contact& c)
{
    if (isContactInGroup(user, gname, c)) return;

    ifstream fin(getGroupsFileName(user));
    if (!checkFileOpen(fin, getGroupsFileName(user))) return;

    ofstream fout("temp.txt");
    if (!checkFileOpen(fout, "temp.txt")) { fin.close(); return; }

    copyGroupBlock(fin, fout, gname, c);

    fin.close();
    fout.close();

    remove(getGroupsFileName(user).c_str());
    if (rename("temp.txt", getGroupsFileName(user).c_str()) != 0)
    {
        return;
    }
    
}

//func to ensure group exist
bool ensureGroupExists(User& user, const string& gname)
{
    if (!groupExists(user, gname))
    {
        return false;
    }
    return true;
}
//func to ensure exsistence of contact
bool ensureContactNotInGroup(User& user, const string& gname, const Contact& c)
{
    if (isContactInGroup(user, gname, c))
    {
        return false;
    }
    return true;
}
//MAIN func to add contact to group
bool addContactToGroup(User& user, const string& gname, const Contact& c)
{
    int count = 0;
    Contact* contacts = loadContacts(user, count);

    bool exists = false;
    for (int i = 0; i < count; i++)
        if (sameContact(contacts[i], c)) exists = true;

    delete[] contacts;

    if (!exists) return false;

    if (ensureGroupExists(user, gname) && ensureContactNotInGroup(user, gname, c))
        appendContactToGroup(user, gname, c);

    return true;
}
//func to ensure exsistence of contact in group
bool ensureContactInGroup(User& user, const string& gname, const Contact& c)
{
    if (!isContactInGroup(user, gname, c))
    {
        return false;
    }
    else
    {
        return true;
    }
}
//func to read group members
// Read a single contact from file, returns false if end of group or file
void writeContactToFile(ofstream& fout, const Contact& c) 
{
    fout << c.firstName << "\n" << c.lastName << "\n" << c.phone << "\n";
}

// Read one contact from file, return false if group end or EOF
bool readGroupMember(ifstream& fin, Contact& c) 
{
    if (!getline(fin, c.firstName) || c.firstName.empty())
    {
        return false;
    }
    if (!getline(fin, c.lastName))
    {
        return false;
    }
    if (!getline(fin, c.phone))
    {
        return false;
    }
    return true;
}
//func to delete contact
bool deleteContactFromGroup(User& user, const string& gname, const Contact& target)
{
    ifstream fin(getGroupsFileName(user));
    if (!checkFileOpen(fin, getGroupsFileName(user)))
    {
        return false;
    }
    ofstream fout("temp.txt");
    if (!checkFileOpen(fout, "temp.txt"))
    {
        fin.close();
        return false;
    }

    string line;
    while (getline(fin, line))
    {
        if (line.empty())
        {
            fout << "\n";
            continue;
        }
        fout << line << "\n";
        // Read each member and skip the one to delete
        if (line == gname)
        {
            Contact c;
            while (readGroupMember(fin, c))
            {
                if (!sameContact(c, target))
                {
                    writeContactToFile(fout, c);
                }
            }
            fout << "\n"; // group end 
        }
    }
    fin.close();
    fout.close();
    //replace the file
    remove(getGroupsFileName(user).c_str());

    if (rename("temp.txt", getGroupsFileName(user).c_str()) != 0)
    {
        return false;
    }   
    return true;
}
//main func to remove contact from group
bool removeContactFromGroup(User& user, const string& gname, const Contact& c)
{
    // Ensure group exists first
    if (!ensureGroupExists(user, gname) || !ensureContactInGroup(user, gname, c))
    {
        return false;
    }
    // Actually remove the contact
    deleteContactFromGroup(user, gname, c);
    return true;
}
//skip group members
void skipGroupMembers(ifstream& fin)
{
    Contact c;
    while (readGroupMember(fin, c))
    {
        continue;
    }
}
//print group members
void printGroupMembers(ifstream& fin)
{
    Contact c;
    while (readGroupMember(fin, c))
    {
        cout << "   " << c.firstName << " " << c.lastName << " (" << c.phone << ")\n";
    }
}
//func to view group names
bool viewGroups(User& user)
{
    ifstream fin(getGroupsFileName(user));
    if (!checkFileOpen(fin, getGroupsFileName(user)))
    {
        return false;
    }
    string line;
    while (getline(fin, line))
    {
        if (line.empty())
        {
            continue;
        }
        cout << "- " << line << "\n";
        //skip group members
        skipGroupMembers(fin);
    }
    fin.close();
    return true;
}
// view group names + members
bool viewGroupsWithMembers(User& user)
{
    ifstream fin(getGroupsFileName(user));
    if (!checkFileOpen(fin, getGroupsFileName(user)))
    {
        return false;
    }
    string line;
    while (getline(fin, line))
    {
        if (line.empty())
        {
            continue;
        }
        cout << "- " << line << ":\n";
        // print members
        printGroupMembers(fin);
    }
    fin.close();
    return true;
}
//send message helper
void sendMessageToMembers(ifstream& fin)
{
    Contact c;
    while (readGroupMember(fin, c))
    {
        cout << "   Message sent to " << c.firstName << " "
            << c.lastName << " (" << c.phone << ")\n";
    }
}
// send message to group
void sendMessageToGroup(User& user, const string& gname)
{
    ifstream fin(getGroupsFileName(user));
    if (!checkFileOpen(fin, getGroupsFileName(user)))
    {
        return;
    }
    string line;
    bool found = false;
    while (getline(fin, line))
    {
        if (line.empty())
        {
            continue;
        }
        //if group found
        if (line == gname)
        {
            cout << "Sending message to group: " << gname << "\n";
            sendMessageToMembers(fin);
            found = true;
            break;
        }
        else
        {
            skipGroupMembers(fin);
        }
    }
    //if not found
    if (!found)
    {
        cout << "Group not found!\n";
    }
    fin.close();
}
int main()
{
    User user;
    int choice;

    cout << "===== Welcome to Contact Management System =====\n";

    while (true)
    {
        cout << "\n1. Sign Up\n";
        cout << "2. Login\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        if (choice == 1)
        {
            if (!takeInput(user))
            {
                cout << "Username and password cannot be empty. Please try again.\n";
                continue;
            }
            else
            {
                if (signUp(user))
                {
                    cout << "Sign Up successful. You can now login.\n";
                }
                else
                {
                    cout << "Error while signing up. Please try again.\n";
                }
            }
        }
        else if (choice == 2)
        {
            if (!takeInput(user))
            {
                cout << "Username and password cannot be empty.\n";
                continue;
            }
            else
            {
                if (loginUser(user))
                {
                    cout << "\nLogin successful. Welcome, " << user.username << "!\n";

                    bool sessionActive = true;
                    while (sessionActive)
                    {
                        printMenu();
                        cout << "Enter your choice: ";
                        cin >> choice;
                        cin.ignore();

                        switch (choice)
                        {
                        case 1: // View contacts
                        {
                            int count = 0;
                            Contact* contacts = viewContact(user, count);
                            if (contacts && count > 0)
                            {
                                sortContacts(contacts, count);
                                cout << "Your contacts:\n";
                                displayContact(contacts, count);
                            }
                            else
                            {
                                cout << "No contacts to display\n";
                            }
                            delete[]contacts;
                            contacts = nullptr;
                            break;
                        }
                        case 2: // Add contact
                        {
                            if (addContact(user))
                            {
                                cout << "Contact added successfully.\n";
                            }
                            else
                            {
                                cout << "Contact not added (duplicate or cancelled).\n";
                            }
                            break;
                        }
                        case 3: // Search contact
                        {
                            string searchTerm;
                            cout << "Enter name to search: ";
                            getline(cin, searchTerm);
                            if (!searchContact(user, searchTerm))
                            {
                                cout << "No matching contact found.\n";
                            }
                            break;
                        }
                        case 4: // Update contact phone
                        {
                            if (updateContactPhoneNumber(user))
                            {
                                cout << "Contact updated successfully.\n";
                            }
                            else
                            {
                                cout << "Contact not found or update failed.\n";
                            }
                            break;
                        }
                        case 5: // Delete contact
                        {
                            if (deleteContact(user))
                            {
                                cout << "Contact deleted successfully.\n";
                            }
                            else
                            {
                                cout << "Contact not found or delete failed.\n";
                            }
                            break;
                        }
                        case 6: // Create Group
                        {
                            string groupName;
                            cout << "Enter group name to create: ";
                            getline(cin, groupName);
                            char addDefault;
                            cout << "Do you want to add a default contact? (y/n): ";
                            cin >> addDefault;
                            cin.ignore();
                            if (addDefault == 'y' || addDefault == 'Y')
                            {
                                Contact defaultContact = takeContactInput();
                                if (createGroup(user, groupName, defaultContact))
                                    cout << "Group created successfully.\n";
                                else
                                    cout << "Group creation failed.\n";
                            }
                            else
                            {
                                if (createGroup(user, groupName))
                                    cout << "Group created successfully.\n";
                                else
                                    cout << "Group creation failed.\n";
                            }
                            break;
                        }
                        case 7: // View Groups
                        {
                            char viewOption;
                            cout << "View (1) Only Group Names or (2) Group Names with Members? ";
                            cin >> viewOption;
                            cin.ignore();
                            if (viewOption == '1')
                            {
                                if (!viewGroups(user))
                                    cout << "No groups available.\n";  // <-- ab main me
                            }
                            else
                            {
                                if (!viewGroupsWithMembers(user))
                                    cout << "No groups with members found.\n"; // <-- ab main me
                            }
                            break;
                        }
                        case 8: // Add / Remove Contact from Group
                        {
                            string groupName;
                            cout << "Enter group name: ";
                            getline(cin, groupName);
                            char action;
                            cout << "Do you want to (a) Add or (r) Remove contact? ";
                            cin >> action;
                            cin.ignore();
                            if (action == 'a' || action == 'A')
                            {
                                Contact c = takeContactInput();
                                if (addContactToGroup(user, groupName, c))
                                    cout << "Contact added to group successfully.\n"; // <-- ab main me
                                else
                                    cout << "Contact not added to group.\n"; // <-- ab main me
                            }
                            else if (action == 'r' || action == 'R')
                            {
                                Contact c = takeContactInput();
                                if (removeContactFromGroup(user, groupName, c))
                                    cout << "Contact removed from group successfully.\n"; // <-- ab main me
                                else
                                    cout << "Contact not removed from group.\n"; // <-- ab main me
                            }
                            else
                            {
                                cout << "Invalid action.\n";
                            }
                            break;
                        }
                        case 9: // Send message to group
                        {
                            string groupName;
                            cout << "Enter group name to send message: ";
                            getline(cin, groupName);
                            sendMessageToGroup(user, groupName);
                            break;
                        }
                        case 10: // Logout
                        {
                            cout << "Logging out " << user.username << "...\n";
                            sessionActive = false;
                            break;
                        }
                        default:
                            cout << "Invalid choice. Please try again.\n";
                            break;
                        }
                    }
                }
                else
                {
                    cout << "Invalid username or password. Please try again.\n";
                }
            }
        }
        else if (choice == 3)
        {
            cout << "Exiting program. Goodbye!\n";
            break;
        }
        else
        {
            cout << "Invalid choice. Try again.\n";
        }
    }

    return 0;
}
