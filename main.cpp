#include "Contact.h"
#include "DirectoryManager.h"
#include <iostream>
#include <string>

// Helper to clear standard input stream
void clearInputStream() {
    // If standard input was in an error state (e.g., EOF or mismatch), clear it
    if (std::cin.fail()) {
        std::cin.clear();
    }
}

// Sub-menu to easily choose/input a contact group
std::string chooseGroup() {
    std::cout << "\nChoose Group:\n";
    std::cout << "1. Family\n";
    std::cout << "2. Friends\n";
    std::cout << "3. Work\n";
    std::cout << "4. Other / Custom\n";
    std::cout << "Enter option (1-4): ";
    
    std::string choice;
    std::getline(std::cin, choice);
    
    if (choice == "1") return "Family";
    if (choice == "2") return "Friends";
    if (choice == "3") return "Work";
    if (choice == "4") {
        std::cout << "Enter Custom Group Name: ";
        std::string custom;
        std::getline(std::cin, custom);
        return custom.empty() ? "Other" : custom;
    }
    return "Other";
}

int main() {
    // Create DirectoryManager pointing to contacts.txt
    DirectoryManager manager("contacts.txt");

    std::string choiceStr;
    int choice = 0;

    do {
        std::cout << "\n==================================================\n";
        std::cout << "        DIGITAL PHONE DIRECTORY SYSTEM\n";
        std::cout << "==================================================\n";
        std::cout << "1.  Add New Contact\n";
        std::cout << "2.  Display All Contacts\n";
        std::cout << "3.  Search Contact (by Name or Phone)\n";
        std::cout << "4.  Edit Contact Details\n";
        std::cout << "5.  Delete a Contact\n";
        std::cout << "6.  Undo Last Deletion\n";
        std::cout << "7.  Display Recently Added Contacts (Queue)\n";
        std::cout << "8.  Display Contacts Group-wise\n";
        std::cout << "9.  Sort Contacts Alphabetically\n";
        std::cout << "10. Save and Exit\n";
        std::cout << "==================================================\n";
        std::cout << "Enter your choice (1-10): ";

        std::getline(std::cin, choiceStr);
        try {
            choice = std::stoi(choiceStr);
        } catch (...) {
            choice = -1; // Set to invalid option if parsing fails
        }

        switch (choice) {
            case 1: {
                std::cout << "\n--- Add New Contact ---\n";
                std::string name, phone, email, address, group;
                bool isFav = false;
                std::string input;

                // Input Name
                do {
                    std::cout << "Enter Name (Required): ";
                    std::getline(std::cin, name);
                    if (name.empty()) {
                        std::cout << "Error: Name cannot be empty!\n";
                    }
                } while (name.empty());

                // Input Phone with validation
                do {
                    std::cout << "Enter Phone Number (10-15 digits, Required): ";
                    std::getline(std::cin, phone);
                    if (!Contact::isValidPhone(phone)) {
                        std::cout << "Error: Invalid phone format! Use only 10 to 15 digits.\n";
                    }
                } while (!Contact::isValidPhone(phone));

                // Input Email with validation (optional)
                do {
                    std::cout << "Enter Email Address (Optional): ";
                    std::getline(std::cin, email);
                    if (!email.empty() && !Contact::isValidEmail(email)) {
                        std::cout << "Error: Invalid email format! (must contain '@' and '.')\n";
                    }
                } while (!email.empty() && !Contact::isValidEmail(email));

                // Input Address
                std::cout << "Enter Address (Optional): ";
                std::getline(std::cin, address);

                // Input Group selection
                group = chooseGroup();

                // Input Favorite Status
                std::cout << "Mark as Favorite? (y/n): ";
                std::getline(std::cin, input);
                isFav = (input == "y" || input == "Y");

                // Create Contact object and add to manager
                Contact newContact(name, phone, email, address, group, isFav);
                if (manager.addContact(newContact)) {
                    std::cout << "\nContact added successfully!\n";
                }
                break;
            }
            case 2:
                manager.displayAllContacts();
                break;
            case 3: {
                std::string query;
                std::cout << "\nEnter Name or Phone Number to Search: ";
                std::getline(std::cin, query);
                manager.searchContact(query);
                break;
            }
            case 4: {
                std::string phone;
                std::cout << "\nEnter Phone Number of the contact to Edit: ";
                std::getline(std::cin, phone);
                manager.editContact(phone);
                break;
            }
            case 5: {
                std::string phone;
                std::cout << "\nEnter Phone Number of the contact to Delete: ";
                std::getline(std::cin, phone);
                manager.deleteContact(phone);
                break;
            }
            case 6:
                manager.undoDelete();
                break;
            case 7:
                manager.displayRecent();
                break;
            case 8: {
                std::cout << "\nChoose Group to Display:\n";
                std::cout << "1. Family\n";
                std::cout << "2. Friends\n";
                std::cout << "3. Work\n";
                std::cout << "4. Custom / Other\n";
                std::cout << "Enter choice: ";
                std::string groupOpt;
                std::getline(std::cin, groupOpt);
                
                std::string groupName;
                if (groupOpt == "1") groupName = "Family";
                else if (groupOpt == "2") groupName = "Friends";
                else if (groupOpt == "3") groupName = "Work";
                else {
                    std::cout << "Enter Group Name: ";
                    std::getline(std::cin, groupName);
                }
                manager.displayGroup(groupName);
                break;
            }
            case 9:
                manager.sortContacts();
                break;
            case 10:
                manager.saveContacts();
                std::cout << "\nExiting Digital Phone Directory. Goodbye!\n";
                break;
            default:
                std::cout << "\nInvalid choice! Please enter a number between 1 and 10.\n";
                break;
        }

    } while (choice != 10);

    return 0;
}
