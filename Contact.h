#ifndef CONTACT_H
#define CONTACT_H

#include <string>

// Contact class representing a single contact in the directory
class Contact {
private:
    std::string name;
    std::string phone;
    std::string email;
    std::string address;
    std::string group;
    bool isFavorite;

public:
    // Default constructor
    Contact();

    // Parameterized constructor
    Contact(std::string name, std::string phone, std::string email, 
            std::string address, std::string group, bool isFavorite);

    // Getters and Setters
    std::string getName() const;
    void setName(const std::string& name);

    std::string getPhone() const;
    void setPhone(const std::string& phone);

    std::string getEmail() const;
    void setEmail(const std::string& email);

    std::string getAddress() const;
    void setAddress(const std::string& address);

    std::string getGroup() const;
    void setGroup(const std::string& group);

    bool getIsFavorite() const;
    void setIsFavorite(bool isFavorite);

    // Displays the contact details in a single formatted row
    void displayRow() const;

    // Helper to validate phone number format (must be 10 digits for simplicity)
    static bool isValidPhone(const std::string& phone);

    // Helper to validate email format (contains '@' and '.')
    static bool isValidEmail(const std::string& email);
};

#endif // CONTACT_H
