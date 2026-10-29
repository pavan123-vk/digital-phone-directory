#ifndef UNDO_STACK_H
#define UNDO_STACK_H

#include "Contact.h"

// Node structure for the Undo Stack
struct StackNode {
    Contact contact;
    StackNode* next;

    StackNode(Contact c) : contact(c), next(nullptr) {}
};

// Stack implementation to support "Undo Delete" operation (LIFO)
class UndoStack {
private:
    StackNode* topNode;

public:
    // Constructor
    UndoStack();

    // Destructor
    ~UndoStack();

    // Push a deleted contact onto the stack
    void push(Contact contact);

    // Pop the most recently deleted contact from the stack.
    // Throws an exception or returns default contact if empty (we will return default contact).
    Contact pop();

    // Check if stack is empty
    bool isEmpty() const;

    // Clear all elements from the stack
    void clear();
};

#endif // UNDO_STACK_H
