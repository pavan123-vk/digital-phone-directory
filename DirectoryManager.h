#ifndef DIRECTORY_MANAGER_H
#define DIRECTORY_MANAGER_H

#include "ContactLinkedList.h"
#include "ContactBST.h"
#include "UndoStack.h"
#include "RecentlyAddedQueue.h"
#include <string>

// Manager class to coordinate operations across the manual data structures
class DirectoryManager {
private:
    ContactLinkedList list;
    ContactBST bst;
    UndoStack undoStack;
    RecentlyAddedQueue recentQueue;
    std::string filename;

public:
    // Constructor specifying storage filename
    DirectoryManager(const std::string& dbFilename);

    // Adds a new contact. prevents duplicate phone numbers.
    // isLoad=true disables pushing to the recently added queue during startup load.
    bool addContact(const Contact& contact, bool isLoad = false);

    // Displays all contacts
    void displayAllContacts() const;

    // Searches contact by Name (using BST) or Phone (using Linked List)
    void searchContact(const std::string& query) const;

    // Finds contact and returns node pointer (for REST API)
    ContactNode* findContact(const std::string& query) const;

    // Edits contact details by phone number
    bool editContact(const std::string& phone);

    // Deletes a contact by phone number and pushes it to the Undo Stack
    bool deleteContact(const std::string& phone);

    // Restores the most recently deleted contact from the Undo Stack
    bool undoDelete(Contact& restoredContactOut);

    // Displays the queue of recently added contacts
    void displayRecent() const;

    // Displays contacts by group name
    void displayGroup(const std::string& groupName) const;

    // Sorts contacts alphabetically and rebuilds the BST to match the sorted order
    void sortContacts();

    // Saves all contacts to the text file in a pipe-delimited format
    void saveContacts() const;

    // Loads all contacts from the text file on startup
    void loadContacts();

    // Accessors for data structures to support API serialization
    const ContactLinkedList& getList() const { return list; }
    const RecentlyAddedQueue& getQueue() const { return recentQueue; }
    const UndoStack& getStack() const { return undoStack; }
    void rebuildBST() { bst.rebuild(list); }
};

#endif // DIRECTORY_MANAGER_H
