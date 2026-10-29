#include "ContactBST.h"
#include <algorithm>
#include <cctype>

// Helper utility to convert a string to lowercase for case-insensitive comparisons
static std::string toLowercase(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) {
        return std::tolower(c);
    });
    return str;
}

// Constructor
ContactBST::ContactBST() : root(nullptr) {}

// Destructor
ContactBST::~ContactBST() {
    clear();
}

// Insert helper (Recursive)
BSTNode* ContactBST::insertHelper(BSTNode* node, std::string name, ContactNode* ref) {
    if (node == nullptr) {
        return new BSTNode(name, ref);
    }

    // Compare names case-insensitively
    std::string lowerName = toLowercase(name);
    std::string lowerNodeName = toLowercase(node->name);

    if (lowerName < lowerNodeName) {
        node->left = insertHelper(node->left, name, ref);
    } else {
        // If names are equal or greater, insert to the right subtree to handle duplicate names
        node->right = insertHelper(node->right, name, ref);
    }
    return node;
}

// Public insert method
void ContactBST::insert(std::string name, ContactNode* ref) {
    root = insertHelper(root, name, ref);
}

// Search helper (Recursive, O(log n) average)
BSTNode* ContactBST::searchHelper(BSTNode* node, std::string name) const {
    if (node == nullptr) {
        return nullptr;
    }

    std::string lowerName = toLowercase(name);
    std::string lowerNodeName = toLowercase(node->name);

    if (lowerName == lowerNodeName) {
        return node;
    } else if (lowerName < lowerNodeName) {
        return searchHelper(node->left, name);
    } else {
        return searchHelper(node->right, name);
    }
}

// Public search method
ContactNode* ContactBST::search(std::string name) const {
    BSTNode* node = searchHelper(root, name);
    if (node != nullptr) {
        return node->contactRef;
    }
    return nullptr;
}

// Clear helper (Recursive postorder traversal)
void ContactBST::clearHelper(BSTNode* node) {
    if (node == nullptr) return;
    clearHelper(node->left);
    clearHelper(node->right);
    delete node;
}

// Public clear method
void ContactBST::clear() {
    clearHelper(root);
    root = nullptr;
}

// Rebuilds the BST from scratch using the Linked List
void ContactBST::rebuild(const ContactLinkedList& list) {
    clear();
    ContactNode* temp = list.getHead();
    while (temp != nullptr) {
        insert(temp->contact.getName(), temp);
        temp = temp->next;
    }
}
