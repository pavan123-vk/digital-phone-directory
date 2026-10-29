#include "httplib.h"
#include "json.hpp"
#include "DirectoryManager.h"
#include "Contact.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>

using json = nlohmann::json;

// Helper to convert Contact to JSON
json contactToJson(const Contact& c) {
    return json{
        {"name", c.getName()},
        {"phone", c.getPhone()},
        {"email", c.getEmail()},
        {"address", c.getAddress()},
        {"group", c.getGroup()},
        {"isFavorite", c.getIsFavorite()}
    };
}

// Helper to construct Contact from JSON
Contact jsonToContact(const json& j) {
    return Contact(
        j.value("name", ""),
        j.value("phone", ""),
        j.value("email", ""),
        j.value("address", ""),
        j.value("group", "Other"),
        j.value("isFavorite", false)
    );
}

int main() {
    // Initialize DirectoryManager
    DirectoryManager manager("contacts.txt");

    // Initialize HTTP server
    httplib::Server svr;

    // CORS preflight configuration for all options requests
    svr.Options(R"(/api/.*)", [](const httplib::Request&, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        res.set_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
        res.status = 200;
    });

    // Global middleware to inject CORS response headers for all requests
    svr.after([](const httplib::Request&, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        res.set_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
    });

    // GET /api/contacts - Retrieve all contacts from main linked list
    svr.Get("/api/contacts", [&](const httplib::Request&, httplib::Response& res) {
        json contactsArr = json::array();
        ContactNode* current = manager.getList().getHead();
        while (current != nullptr) {
            contactsArr.push_back(contactToJson(current->contact));
            current = current->next;
        }
        res.set_content(contactsArr.dump(), "application/json");
    });

    // GET /api/contacts/search?q=query - Search contact by name (BST) or phone (Linked List)
    svr.Get("/api/contacts/search", [&](const httplib::Request& req, httplib::Response& res) {
        std::string query = req.get_param_value("q");
        if (query.empty()) {
            res.status = 400;
            res.set_content(json{{"error", "Query parameter 'q' is required"}}.dump(), "application/json");
            return;
        }

        ContactNode* foundNode = manager.findContact(query);
        if (foundNode != nullptr) {
            res.set_content(contactToJson(foundNode->contact).dump(), "application/json");
        } else {
            res.status = 404;
            res.set_content(json{{"error", "Contact not found"}}.dump(), "application/json");
        }
    });

    // GET /api/contacts/recent - Get rolling list of recently added contacts (Queue)
    svr.Get("/api/contacts/recent", [&](const httplib::Request&, httplib::Response& res) {
        json recentArr = json::array();
        QueueNode* current = manager.getQueue().getFront();
        while (current != nullptr) {
            recentArr.push_back(contactToJson(current->contact));
            current = current->next;
        }
        res.set_content(recentArr.dump(), "application/json");
    });

    // POST /api/contacts - Add a new contact
    svr.Post("/api/contacts", [&](const httplib::Request& req, httplib::Response& res) {
        try {
            auto j = json::parse(req.body);
            Contact c = jsonToContact(j);

            // Validation checks
            if (c.getName().empty() || c.getPhone().empty()) {
                res.status = 400;
                res.set_content(json{{"error", "Name and Phone Number are required fields"}}.dump(), "application/json");
                return;
            }
            if (!Contact::isValidPhone(c.getPhone())) {
                res.status = 400;
                res.set_content(json{{"error", "Invalid phone format (must be 10-15 digits)"}}.dump(), "application/json");
                return;
            }
            if (!c.getEmail().empty() && !Contact::isValidEmail(c.getEmail())) {
                res.status = 400;
                res.set_content(json{{"error", "Invalid email format"}}.dump(), "application/json");
                return;
            }
            if (manager.getList().exists(c.getPhone())) {
                res.status = 409;
                res.set_content(json{{"error", "Phone number already exists"}}.dump(), "application/json");
                return;
            }

            if (manager.addContact(c)) {
                manager.saveContacts(); // Persist immediately
                res.status = 201;
                res.set_content(contactToJson(c).dump(), "application/json");
            } else {
                res.status = 500;
                res.set_content(json{{"error", "Failed to insert contact into directory"}}.dump(), "application/json");
            }
        } catch (const std::exception& e) {
            res.status = 400;
            res.set_content(json{{"error", std::string("Malformed JSON payload: ") + e.what()}}.dump(), "application/json");
        }
    });

    // PUT /api/contacts/:phone - Edit an existing contact by phone number
    svr.Put(R"(/api/contacts/([^/]+))", [&](const httplib::Request& req, httplib::Response& res) {
        std::string targetPhone = req.matches[1].str();
        try {
            auto j = json::parse(req.body);
            ContactNode* node = manager.getList().searchByPhone(targetPhone);
            if (node == nullptr) {
                res.status = 404;
                res.set_content(json{{"error", "Contact not found"}}.dump(), "application/json");
                return;
            }

            std::string name = j.value("name", node->contact.getName());
            std::string phone = j.value("phone", node->contact.getPhone());
            std::string email = j.value("email", node->contact.getEmail());
            std::string address = j.value("address", node->contact.getAddress());
            std::string group = j.value("group", node->contact.getGroup());
            bool isFavorite = j.value("isFavorite", node->contact.getIsFavorite());

            // Validations
            if (name.empty()) {
                res.status = 400;
                res.set_content(json{{"error", "Name cannot be empty"}}.dump(), "application/json");
                return;
            }
            if (!Contact::isValidPhone(phone)) {
                res.status = 400;
                res.set_content(json{{"error", "Invalid phone number format"}}.dump(), "application/json");
                return;
            }
            if (!email.empty() && !Contact::isValidEmail(email)) {
                res.status = 400;
                res.set_content(json{{"error", "Invalid email address format"}}.dump(), "application/json");
                return;
            }

            // Duplication check if phone changed
            if (phone != targetPhone && manager.getList().exists(phone)) {
                res.status = 409;
                res.set_content(json{{"error", "New phone number is already assigned to another contact"}}.dump(), "application/json");
                return;
            }

            // Update contact data
            node->contact.setName(name);
            node->contact.setPhone(phone);
            node->contact.setEmail(email);
            node->contact.setAddress(address);
            node->contact.setGroup(group);
            node->contact.setIsFavorite(isFavorite);

            // Re-sync the search trees and save state
            manager.rebuildBST();
            manager.saveContacts();

            res.set_content(contactToJson(node->contact).dump(), "application/json");
        } catch (const std::exception& e) {
            res.status = 400;
            res.set_content(json{{"error", std::string("Malformed JSON payload: ") + e.what()}}.dump(), "application/json");
        }
    });

    // DELETE /api/contacts/:phone - Delete a contact
    svr.Delete(R"(/api/contacts/([^/]+))", [&](const httplib::Request& req, httplib::Response& res) {
        std::string targetPhone = req.matches[1].str();
        if (manager.deleteContact(targetPhone)) {
            manager.saveContacts();
            res.set_content(json{{"success", true}}.dump(), "application/json");
        } else {
            res.status = 404;
            res.set_content(json{{"error", "Contact not found"}}.dump(), "application/json");
        }
    });

    // POST /api/contacts/undo - Restore the last deleted contact (Stack)
    svr.Post("/api/contacts/undo", [&](const httplib::Request&, httplib::Response& res) {
        Contact restored;
        if (manager.undoDelete(restored)) {
            manager.saveContacts();
            res.set_content(contactToJson(restored).dump(), "application/json");
        } else {
            res.status = 400;
            res.set_content(json{{"error", "Undo stack is empty or number conflicts exist"}}.dump(), "application/json");
        }
    });

    // POST /api/contacts/sort - Alphabetically sort contacts (Linked List Bubble Sort)
    svr.Post("/api/contacts/sort", [&](const httplib::Request&, httplib::Response& res) {
        manager.sortContacts();
        manager.saveContacts();

        // Return sorted list
        json contactsArr = json::array();
        ContactNode* current = manager.getList().getHead();
        while (current != nullptr) {
            contactsArr.push_back(contactToJson(current->contact));
            current = current->next;
        }
        res.set_content(contactsArr.dump(), "application/json");
    });

    // GET /api/contacts/status - Check if Undo Stack is empty
    svr.Get("/api/contacts/status", [&](const httplib::Request&, httplib::Response& res) {
        res.set_content(json{
            {"canUndo", !manager.getStack().isEmpty()}
        }.dump(), "application/json");
    });

    // Start server listening on port 5000
    std::cout << "Starting C++ REST API Server on http://localhost:5000...\n";
    if (!svr.listen("127.0.0.1", 5000)) {
        std::cerr << "Error: Failed to bind C++ Server to localhost:5000\n";
        return 1;
    }

    return 0;
}
