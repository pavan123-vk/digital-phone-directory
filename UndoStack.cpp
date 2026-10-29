#include "UndoStack.h"
#include <iostream>

// Constructor
UndoStack::UndoStack() : topNode(nullptr) {}

// Destructor
UndoStack::~UndoStack() {
    clear();
}

// Push a contact onto the stack
void UndoStack::push(Contact contact) {
    StackNode* newNode = new StackNode(contact);
    newNode->next = topNode;
    topNode = newNode;
}

// Pop and return the top contact
Contact UndoStack::pop() {
    if (isEmpty()) {
        // Return an empty/default contact if stack is empty
        return Contact();
    }
    StackNode* temp = topNode;
    Contact poppedContact = temp->contact;
    topNode = topNode->next;
    delete temp;
    return poppedContact;
}

// Check if stack is empty
bool UndoStack::isEmpty() const {
    return topNode == nullptr;
}

// Clear stack and free memory
void UndoStack::clear() {
    while (!isEmpty()) {
        pop();
    }
}
