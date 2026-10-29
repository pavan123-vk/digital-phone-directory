#ifndef RECENTLY_ADDED_QUEUE_H
#define RECENTLY_ADDED_QUEUE_H

#include "Contact.h"

// Node structure for the Recently Added Queue
struct QueueNode {
    Contact contact;
    QueueNode* next;

    QueueNode(Contact c) : contact(c), next(nullptr) {}
};

// Queue implementation to track Recently Added Contacts (FIFO)
class RecentlyAddedQueue {
private:
    QueueNode* frontNode;
    QueueNode* rearNode;
    int currentSize;
    int maxSize;

public:
    // Constructor with default maximum size of 5
    RecentlyAddedQueue(int maxSize = 5);

    // Destructor
    ~RecentlyAddedQueue();

    // Add a contact to the queue. If queue exceeds maxSize, oldest contact is removed.
    void enqueue(Contact contact);

    // Remove the oldest contact from the queue
    void dequeue();

    // Check if the queue is empty
    bool isEmpty() const;

    // Display all recently added contacts
    void display() const;

    // Clear the queue
    void clear();

    // Getter for frontNode to support API serialization
    QueueNode* getFront() const { return frontNode; }
};

#endif // RECENTLY_ADDED_QUEUE_H
