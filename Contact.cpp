#include "Contact.h"
#include <iostream>
#include <iomanip>
#include <cctype>

// Default constructor initializing fields to empty/default values
Contact::Contact() : name(""), phone(""), email(""), address(""), group("Other"), isFavorite(false) {}

// Parameterized constructor to initialize contact with custom details
Contact::Contact(std::string name, std::string phone, std::string email, 
                 std::string address, std::string group, bool isFavorite) {
    this->name = name;
    this->phone = phone;
    this->email = email;
    this->address = address;
    this->group = (group.empty()) ? "Other" : group;
    this->isFavorite = isFavorite;
}

// Getters and Setters
std::string Contact::getName() const { return name; }
void Contact::setName(const std::string& name) { this->name = name; }

std::string Contact::getPhone() const { return phone; }
void Contact::setPhone(const std::string& phone) { this->phone = phone; }

std::string Contact::getEmail() const { return email; }
void Contact::setEmail(const std::string& email) { this->email = email; }

std::string Contact::getAddress() const { return address; }
void Contact::setAddress(const std::string& address) { this->address = address; }

std::string Contact::getGroup() const { return group; }
void Contact::setGroup(const std::string& group) { this->group = (group.empty()) ? "Other" : group; }

bool Contact::getIsFavorite() const { return isFavorite; }
void Contact::setIsFavorite(bool isFavorite) { this->isFavorite = isFavorite; }

// Displays contact information in a neat tabular row
void Contact::displayRow() const {
    std::string favStatus = isFavorite ? "[★] Yes" : "No";
    std::cout << std::left 
              << std::setw(20) << name 
              << std::setw(15) << phone 
              << std::setw(25) << email 
              << std::setw(15) << group 
              << std::setw(10) << favStatus 
              << std::setw(25) << address 
              << std::endl;
}

// Validates that the phone number contains only digits and is between 10 and 15 characters
bool Contact::isValidPhone(const std::string& phone) {
    if (phone.length() < 10 || phone.length() > 15) {
        return false;
    }
    for (char const &c : phone) {
        if (!std::isdigit(c)) {
            return false;
        }
    }
    return true;
}

// Validates that the email address contains '@' and '.' characters with '@' appearing before '.'
bool Contact::isValidEmail(const std::string& email) {
    if (email.empty()) return true; // Email can be optional/empty
    size_t atPos = email.find('@');
    size_t dotPos = email.rfind('.');
    return (atPos != std::string::npos && dotPos != std::string::npos && atPos < dotPos && atPos > 0 && dotPos < email.length() - 1);
}
