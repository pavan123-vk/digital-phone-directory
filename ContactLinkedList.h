#ifndef CONTACT_LINKED_LIST_H
#define CONTACT_LINKED_LIST_H

#include "Contact.h"

// Node structure for the Singly Linked List
struct ContactNode {
    Contact contact;
    ContactNode* next;

    ContactNode(Contact c) : contact(c), next(nullptr) {}
};

// Linked List to manage main database of contacts
class ContactLinkedList {
private:
    ContactNode* head;

public:
    // Constructor
    ContactLinkedList();

    // Destructor to free dynamically allocated memory
    ~ContactLinkedList();

    // Insert a new contact at the end of the list
    void insert(Contact contact);

    // Delete a contact by phone number. Returns true if deleted, false if not found.
    bool remove(const std::string& phone, Contact& deletedContactOut);

    // Search contact by phone number (Linear search, O(n))
    ContactNode* searchByPhone(const std::string& phone) const;

    // Checks if a phone number already exists in the directory
    bool exists(const std::string& phone) const;

    // Displays all contacts in a formatted table
    void displayAll() const;

    // Displays contacts belonging to a specific group
    void displayGroup(const std::string& groupName) const;

    // Sorts the linked list alphabetically by contact name using Bubble Sort
    void sortAlphabetically();

    // Get the head node of the linked list
    ContactNode* getHead() const;

    // Clears the list and deletes all nodes
    void clear();
};

#endif // CONTACT_LINKED_LIST_H
