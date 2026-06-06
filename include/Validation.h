#pragma once
#include <string>

namespace Validation {
    bool isValidId(const std::string& id);
    bool isValidName(const std::string& name);
    bool isValidSalary(const std::string& input, double& out);
    bool isValidRating(const std::string& input, float& out);
    bool isValidDate(const std::string& date);
    bool isValidPhone(const std::string& phone);
    bool isValidEmail(const std::string& email);
    bool isNonEmpty(const std::string& s);

    // Trim whitespace from both ends
    std::string trim(const std::string& s);

    // Case-insensitive substring search
    bool containsIgnoreCase(const std::string& haystack, const std::string& needle);
}
