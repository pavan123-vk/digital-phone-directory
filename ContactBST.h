#ifndef CONTACT_BST_H
#define CONTACT_BST_H

#include "ContactLinkedList.h"
#include <string>

// Node structure for the Binary Search Tree
struct BSTNode {
    std::string name;
    ContactNode* contactRef; // Pointer to the actual contact node in the Linked List
    BSTNode* left;
    BSTNode* right;

    BSTNode(std::string n, ContactNode* ref) : name(n), contactRef(ref), left(nullptr), right(nullptr) {}
};

// Binary Search Tree to enable fast searching of contacts by Name (O(log n) average case)
class ContactBST {
private:
    BSTNode* root;

    // Helper method to insert a node recursively
    BSTNode* insertHelper(BSTNode* node, std::string name, ContactNode* ref);

    // Helper method to search for a name recursively
    BSTNode* searchHelper(BSTNode* node, std::string name) const;

    // Helper method to deallocate memory recursively
    void clearHelper(BSTNode* node);

public:
    // Constructor
    ContactBST();

    // Destructor
    ~ContactBST();

    // Insert a contact name and its node reference into the BST
    void insert(std::string name, ContactNode* ref);

    // Search for a contact by name. Returns reference to the ContactNode if found, nullptr otherwise.
    ContactNode* search(std::string name) const;

    // Clear the tree
    void clear();

    // Rebuild the entire BST using the current state of the Linked List
    // This is called when contacts are deleted, edited, or sorted.
    void rebuild(const ContactLinkedList& list);
};

#endif // CONTACT_BST_H
