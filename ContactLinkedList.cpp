#include "ContactLinkedList.h"
#include <iostream>
#include <iomanip>

// Constructor
ContactLinkedList::ContactLinkedList() : head(nullptr) {}

// Destructor
ContactLinkedList::~ContactLinkedList() {
    clear();
}

// Insert a contact at the end of the linked list
void ContactLinkedList::insert(Contact contact) {
    ContactNode* newNode = new ContactNode(contact);
    if (!head) {
        head = newNode;
        return;
    }
    ContactNode* temp = head;
    while (temp->next != nullptr) {
        temp = temp->next;
    }
    temp->next = newNode;
}

// Removes a contact by phone number and returns the contact details in deletedContactOut
bool ContactLinkedList::remove(const std::string& phone, Contact& deletedContactOut) {
    if (!head) return false;

    ContactNode* temp = head;
    ContactNode* prev = nullptr;

    // Check if the head node matches
    if (temp != nullptr && temp->contact.getPhone() == phone) {
        head = temp->next;
        deletedContactOut = temp->contact;
        delete temp;
        return true;
    }

    // Search for the node to delete
    while (temp != nullptr && temp->contact.getPhone() != phone) {
        prev = temp;
        temp = temp->next;
    }

    // If node was not found
    if (temp == nullptr) return false;

    // Unlink the node from linked list
    prev->next = temp->next;
    deletedContactOut = temp->contact;
    delete temp;
    return true;
}

// Linearly searches the linked list for a contact with the matching phone number
ContactNode* ContactLinkedList::searchByPhone(const std::string& phone) const {
    ContactNode* temp = head;
    while (temp != nullptr) {
        if (temp->contact.getPhone() == phone) {
            return temp;
        }
        temp = temp->next;
    }
    return nullptr;
}

// Checks if the phone number already exists
bool ContactLinkedList::exists(const std::string& phone) const {
    return searchByPhone(phone) != nullptr;
}

// Displays all contacts in a formatted table
void ContactLinkedList::displayAll() const {
    if (!head) {
        std::cout << "\nNo contacts found in the directory!\n";
        return;
    }

    // Print table header
    std::cout << "\n" << std::string(100, '-') << "\n";
    std::cout << std::left 
              << std::setw(20) << "Name" 
              << std::setw(15) << "Phone" 
              << std::setw(25) << "Email" 
              << std::setw(15) << "Group" 
              << std::setw(10) << "Favorite" 
              << std::setw(25) << "Address" 
              << "\n";
    std::cout << std::string(100, '-') << "\n";

    ContactNode* temp = head;
    while (temp != nullptr) {
        temp->contact.displayRow();
        temp = temp->next;
    }
    std::cout << std::string(100, '-') << "\n";
}

// Displays contacts belonging to a specific group
void ContactLinkedList::displayGroup(const std::string& groupName) const {
    if (!head) {
        std::cout << "\nNo contacts found in the directory!\n";
        return;
    }

    bool found = false;
    ContactNode* temp = head;
    
    // Check if any contact belongs to the group first
    while (temp != nullptr) {
        // Simple case-insensitive check could be done, but standard equality check is fine.
        // We will do a basic comparison.
        if (temp->contact.getGroup() == groupName) {
            if (!found) {
                // Print table header on first match
                std::cout << "\n--- Group: " << groupName << " ---\n";
                std::cout << std::string(100, '-') << "\n";
                std::cout << std::left 
                          << std::setw(20) << "Name" 
                          << std::setw(15) << "Phone" 
                          << std::setw(25) << "Email" 
                          << std::setw(15) << "Group" 
                          << std::setw(10) << "Favorite" 
                          << std::setw(25) << "Address" 
                          << "\n";
                std::cout << std::string(100, '-') << "\n";
                found = true;
            }
            temp->contact.displayRow();
        }
        temp = temp->next;
    }

    if (found) {
        std::cout << std::string(100, '-') << "\n";
    } else {
        std::cout << "\nNo contacts found in the group '" << groupName << "'!\n";
    }
}

// Bubble Sort on the linked list (swapping data within nodes for simplicity and reliability)
void ContactLinkedList::sortAlphabetically() {
    if (!head || !head->next) return;

    bool swapped;
    ContactNode* ptr1;
    ContactNode* lptr = nullptr;

    do {
        swapped = false;
        ptr1 = head;

        while (ptr1->next != lptr) {
            // Compare names lexicographically
            if (ptr1->contact.getName() > ptr1->next->contact.getName()) {
                // Swap the contact structures in nodes
                Contact temp = ptr1->contact;
                ptr1->contact = ptr1->next->contact;
                ptr1->next->contact = temp;
                swapped = true;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);
}

// Get the head node
ContactNode* ContactLinkedList::getHead() const {
    return head;
}

// Clears the list by deleting nodes one by one
void ContactLinkedList::clear() {
    ContactNode* current = head;
    ContactNode* nextNode = nullptr;

    while (current != nullptr) {
        nextNode = current->next;
        delete current;
        current = nextNode;
    }
    head = nullptr;
}
