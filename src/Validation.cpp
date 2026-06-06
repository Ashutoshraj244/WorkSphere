#include "Validation.h"
#include <algorithm>
#include <cctype>
#include <sstream>
#include <stdexcept>

namespace Validation {

std::string trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

bool isNonEmpty(const std::string& s) {
    return !trim(s).empty();
}

bool isValidId(const std::string& id) {
    if (id.size() < 3 || id.size() > 12) return false;
    for (char c : id) {
        if (!std::isalnum(c) && c != '-' && c != '_') return false;
    }
    return true;
}

bool isValidName(const std::string& name) {
    std::string t = trim(name);
    if (t.size() < 2 || t.size() > 60) return false;
    for (char c : t) {
        if (!std::isalpha(c) && c != ' ' && c != '-' && c != '\'') return false;
    }
    return true;
}

bool isValidSalary(const std::string& input, double& out) {
    try {
        size_t pos;
        double val = std::stod(input, &pos);
        if (pos != input.size()) return false;
        if (val < 0 || val > 10000000) return false;
        out = val;
        return true;
    } catch (...) {
        return false;
    }
}

bool isValidRating(const std::string& input, float& out) {
    try {
        size_t pos;
        float val = std::stof(input, &pos);
        if (pos != input.size()) return false;
        if (val < 0.0f || val > 5.0f) return false;
        out = val;
        return true;
    } catch (...) {
        return false;
    }
}

bool isValidDate(const std::string& date) {
    // Expects YYYY-MM-DD
    if (date.size() != 10) return false;
    if (date[4] != '-' || date[7] != '-') return false;
    for (int i : {0,1,2,3,5,6,8,9}) {
        if (!std::isdigit(date[i])) return false;
    }
    int month = std::stoi(date.substr(5, 2));
    int day   = std::stoi(date.substr(8, 2));
    return month >= 1 && month <= 12 && day >= 1 && day <= 31;
}

bool isValidPhone(const std::string& phone) {
    std::string t = trim(phone);
    if (t.size() < 7 || t.size() > 15) return false;
    for (char c : t) {
        if (!std::isdigit(c) && c != '+' && c != '-' && c != ' ' && c != '(' && c != ')') return false;
    }
    return true;
}

bool isValidEmail(const std::string& email) {
    size_t at = email.find('@');
    if (at == std::string::npos || at == 0) return false;
    size_t dot = email.find('.', at);
    if (dot == std::string::npos || dot == at + 1 || dot == email.size() - 1) return false;
    return email.size() <= 100;
}

bool containsIgnoreCase(const std::string& haystack, const std::string& needle) {
    if (needle.empty()) return true;
    std::string h = haystack, n = needle;
    std::transform(h.begin(), h.end(), h.begin(), ::tolower);
    std::transform(n.begin(), n.end(), n.begin(), ::tolower);
    return h.find(n) != std::string::npos;
}

} // namespace Validation
