#include "RecentlyAddedQueue.h"
#include <iostream>
#include <iomanip>

// Constructor
RecentlyAddedQueue::RecentlyAddedQueue(int maxSize) 
    : frontNode(nullptr), rearNode(nullptr), currentSize(0), maxSize(maxSize) {}

// Destructor
RecentlyAddedQueue::~RecentlyAddedQueue() {
    clear();
}

// Add a contact to the queue (ensuring size limit is maintained)
void RecentlyAddedQueue::enqueue(Contact contact) {
    // If we reached max capacity, dequeue the oldest element
    if (currentSize >= maxSize) {
        dequeue();
    }

    QueueNode* newNode = new QueueNode(contact);
    if (isEmpty()) {
        frontNode = rearNode = newNode;
    } else {
        rearNode->next = newNode;
        rearNode = newNode;
    }
    currentSize++;
}

// Remove the oldest contact (front of queue)
void RecentlyAddedQueue::dequeue() {
    if (isEmpty()) return;

    QueueNode* temp = frontNode;
    frontNode = frontNode->next;

    if (frontNode == nullptr) {
        rearNode = nullptr;
    }

    delete temp;
    currentSize--;
}

// Check if queue is empty
bool RecentlyAddedQueue::isEmpty() const {
    return frontNode == nullptr;
}

// Displays all contacts in the queue (chronological order)
void RecentlyAddedQueue::display() const {
    if (isEmpty()) {
        std::cout << "\nNo recently added contacts to display!\n";
        return;
    }

    std::cout << "\n" << std::string(100, '-') << "\n";
    std::cout << "--- Recently Added Contacts (Queue - Oldest to Newest) ---\n";
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

    QueueNode* temp = frontNode;
    while (temp != nullptr) {
        temp->contact.displayRow();
        temp = temp->next;
    }
    std::cout << std::string(100, '-') << "\n";
}

// Clears the queue
void RecentlyAddedQueue::clear() {
    while (!isEmpty()) {
        dequeue();
    }
}
