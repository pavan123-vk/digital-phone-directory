#include "DirectoryManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

// Constructor
DirectoryManager::DirectoryManager(const std::string& dbFilename) : filename(dbFilename) {
    // Automatically load contacts on startup
    loadContacts();
}

// Adds a contact to the directory. Prevents duplicates.
bool DirectoryManager::addContact(const Contact& contact, bool isLoad) {
    // Prevent duplicate phone numbers
    if (list.exists(contact.getPhone())) {
        std::cout << "\nError: Contact with phone number '" << contact.getPhone() << "' already exists!\n";
        return false;
    }

    // Insert into the Linked List
    list.insert(contact);

    // Get reference to the newly inserted node in the linked list
    ContactNode* ref = list.searchByPhone(contact.getPhone());

    // Insert into BST for O(log n) searching
    if (ref != nullptr) {
        bst.insert(contact.getName(), ref);
    }

    // Add to the recently added queue (skip if loading from file on startup)
    if (!isLoad) {
        recentQueue.enqueue(contact);
    }

    return true;
}

// Display all contacts
void DirectoryManager::displayAllContacts() const {
    list.displayAll();
}

// Searches contacts by name (using BST) or phone (using Linked List)
void DirectoryManager::searchContact(const std::string& query) const {
    if (query.empty()) {
        std::cout << "\nSearch query cannot be empty!\n";
        return;
    }

    // Determine if query is phone number or name
    bool isPhoneQuery = true;
    for (char c : query) {
        if (!std::isdigit(c) && c != '+' && c != '-' && c != ' ') {
            isPhoneQuery = false;
            break;
        }
    }

    ContactNode* result = nullptr;

    if (isPhoneQuery) {
        std::cout << "\nSearching for phone number '" << query << "' in Linked List (Linear Search)...";
        result = list.searchByPhone(query);
    } else {
        std::cout << "\nSearching for name '" << query << "' in Binary Search Tree (Fast O(log n) Search)...";
        result = bst.search(query);
    }

    if (result != nullptr) {
        std::cout << "\nContact Found!\n";
        std::cout << std::string(100, '=') << "\n";
        std::cout << std::left 
                  << std::setw(20) << "Name" 
                  << std::setw(15) << "Phone" 
                  << std::setw(25) << "Email" 
                  << std::setw(15) << "Group" 
                  << std::setw(10) << "Favorite" 
                  << std::setw(25) << "Address" 
                  << "\n";
        std::cout << std::string(100, '=') << "\n";
        result->contact.displayRow();
        std::cout << std::string(100, '=') << "\n";
    } else {
        std::cout << "\nContact not found!\n";
    }
}

// Finds contact and returns node pointer (for REST API)
ContactNode* DirectoryManager::findContact(const std::string& query) const {
    if (query.empty()) return nullptr;

    bool isPhoneQuery = true;
    for (char c : query) {
        if (!std::isdigit(c) && c != '+' && c != '-' && c != ' ') {
            isPhoneQuery = false;
            break;
        }
    }

    if (isPhoneQuery) {
        return list.searchByPhone(query);
    } else {
        return bst.search(query);
    }
}

// Edits a contact by phone number
bool DirectoryManager::editContact(const std::string& phone) {
    ContactNode* node = list.searchByPhone(phone);
    if (node == nullptr) {
        std::cout << "\nError: Contact with phone number '" << phone << "' not found!\n";
        return false;
    }

    std::string input;
    std::cout << "\n--- Editing Contact: " << node->contact.getName() << " ---\n";
    std::cout << "Leave field empty and press Enter to keep the current value.\n";

    // 1. Edit Name
    std::cout << "Enter new name [" << node->contact.getName() << "]: ";
    std::getline(std::cin, input);
    if (!input.empty()) {
        node->contact.setName(input);
    }

    // 2. Edit Phone
    std::cout << "Enter new phone [" << node->contact.getPhone() << "]: ";
    std::getline(std::cin, input);
    if (!input.empty() && input != node->contact.getPhone()) {
        if (!Contact::isValidPhone(input)) {
            std::cout << "Error: Invalid phone number format! Edit aborted.\n";
            return false;
        }
        if (list.exists(input)) {
            std::cout << "Error: Another contact with phone number '" << input << "' already exists! Edit aborted.\n";
            return false;
        }
        node->contact.setPhone(input);
    }

    // 3. Edit Email
    std::cout << "Enter new email [" << node->contact.getEmail() << "]: ";
    std::getline(std::cin, input);
    if (!input.empty()) {
        if (!Contact::isValidEmail(input)) {
            std::cout << "Error: Invalid email format! Edit aborted.\n";
            return false;
        }
        node->contact.setEmail(input);
    }

    // 4. Edit Group
    std::cout << "Enter new group [" << node->contact.getGroup() << "]: ";
    std::getline(std::cin, input);
    if (!input.empty()) {
        node->contact.setGroup(input);
    }

    // 5. Edit Favorite Status
    std::cout << "Set as favorite? (y/n) [Current: " << (node->contact.getIsFavorite() ? "Yes" : "No") << "]: ";
    std::getline(std::cin, input);
    if (!input.empty()) {
        node->contact.setIsFavorite(input == "y" || input == "Y");
    }

    // 6. Edit Address
    std::cout << "Enter new address [" << node->contact.getAddress() << "]: ";
    std::getline(std::cin, input);
    if (!input.empty()) {
        node->contact.setAddress(input);
    }

    // Since the details (potentially name) changed, rebuild the BST to keep search synced
    bst.rebuild(list);
    std::cout << "\nContact updated successfully!\n";
    return true;
}

// Deletes a contact by phone number and saves it on the stack for undoing
bool DirectoryManager::deleteContact(const std::string& phone) {
    Contact deletedContact;
    if (list.remove(phone, deletedContact)) {
        // Push the deleted contact onto the stack for undoing
        undoStack.push(deletedContact);
        // Rebuild the BST to remove reference to the deleted node
        bst.rebuild(list);
        std::cout << "\nContact '" << deletedContact.getName() << "' deleted successfully.\n";
        std::cout << "Note: You can undo this deletion using the Undo option.\n";
        return true;
    }
    std::cout << "\nError: Contact with phone number '" << phone << "' not found!\n";
    return false;
}

// Restores the most recently deleted contact from the stack
bool DirectoryManager::undoDelete(Contact& restoredContactOut) {
    if (undoStack.isEmpty()) {
        std::cout << "\nNo deletions to undo!\n";
        return false;
    }

    Contact restoredContact = undoStack.pop();
    
    // Check if the phone number was already reused while it was deleted
    if (list.exists(restoredContact.getPhone())) {
        std::cout << "\nCannot undo: A contact with phone number '" 
                  << restoredContact.getPhone() << "' was created after deletion.\n";
        // Push it back onto stack so it isn't lost
        undoStack.push(restoredContact);
        return false;
    }

    // Re-insert into the list
    list.insert(restoredContact);
    
    // Re-insert into BST
    ContactNode* ref = list.searchByPhone(restoredContact.getPhone());
    if (ref != nullptr) {
        bst.insert(restoredContact.getName(), ref);
    }

    restoredContactOut = restoredContact;
    std::cout << "\nUndo Successful! Restored contact: " << restoredContact.getName() << "\n";
    return true;
}

// Display recently added contacts
void DirectoryManager::displayRecent() const {
    recentQueue.display();
}

// Display group wise contacts
void DirectoryManager::displayGroup(const std::string& groupName) const {
    list.displayGroup(groupName);
}

// Sort contacts alphabetically
void DirectoryManager::sortContacts() {
    std::cout << "\nSorting contacts alphabetically...\n";
    list.sortAlphabetically();
    // Rebuild the BST to match the sorted references
    bst.rebuild(list);
    std::cout << "Contacts sorted successfully!\n";
}

// Save all contacts from list to text file
void DirectoryManager::saveContacts() const {
    std::ofstream outFile(filename);
    if (!outFile) {
        std::cout << "\nError: Could not save contacts to file '" << filename << "'!\n";
        return;
    }

    ContactNode* temp = list.getHead();
    int count = 0;
    while (temp != nullptr) {
        outFile << temp->contact.getName() << "|"
                << temp->contact.getPhone() << "|"
                << temp->contact.getEmail() << "|"
                << temp->contact.getAddress() << "|"
                << temp->contact.getGroup() << "|"
                << (temp->contact.getIsFavorite() ? "1" : "0") << "\n";
        temp = temp->next;
        count++;
    }
    outFile.close();
    std::cout << "\nSuccessfully saved " << count << " contacts to '" << filename << "'.\n";
}

// Load contacts from text file to directory
void DirectoryManager::loadContacts() {
    std::ifstream inFile(filename);
    if (!inFile) {
        // If file does not exist, it's fine (first run of application)
        return;
    }

    list.clear();
    bst.clear();

    std::string line;
    int count = 0;
    while (std::getline(inFile, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string name, phone, email, address, group, favStr;

        std::getline(ss, name, '|');
        std::getline(ss, phone, '|');
        std::getline(ss, email, '|');
        std::getline(ss, address, '|');
        std::getline(ss, group, '|');
        std::getline(ss, favStr, '|');

        bool isFavorite = (favStr == "1");

        Contact loadedContact(name, phone, email, address, group, isFavorite);
        // Directly add to list (bypassing duplicate checks for performance, assuming clean file)
        list.insert(loadedContact);
        count++;
    }
    inFile.close();

    // Build the BST references for loaded contacts
    bst.rebuild(list);
    std::cout << "\nSuccessfully loaded " << count << " contacts from '" << filename << "' on startup.\n";
}
