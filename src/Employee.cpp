#include "Employee.h"
#include "Validation.h"
#include <sstream>
#include <vector>

// Simple CSV escape: replace | with escaped form (we use | as delimiter)
static std::string csvEscape(const std::string& s) {
    std::string out;
    for (char c : s) {
        if (c == '|') out += "\\|";
        else if (c == '\\') out += "\\\\";
        else out += c;
    }
    return out;
}


std::string Employee::toCSV() const {
    std::ostringstream ss;
    ss << csvEscape(id) << "|"
       << csvEscape(name) << "|"
       << csvEscape(department) << "|"
       << csvEscape(designation) << "|"
       << salary << "|"
       << csvEscape(joiningDate) << "|"
       << csvEscape(phone) << "|"
       << csvEscape(email) << "|"
       << performanceRating;
    return ss.str();
}

Employee Employee::fromCSV(const std::string& line) {
    std::vector<std::string> fields;
    std::string cur;
    for (size_t i = 0; i < line.size(); ++i) {
        if (line[i] == '\\' && i + 1 < line.size()) {
            cur += line[i + 1];
            ++i;
        } else if (line[i] == '|') {
            fields.push_back(cur);
            cur.clear();
        } else {
            cur += line[i];
        }
    }
    fields.push_back(cur);

    if (fields.size() != 9) return Employee();

    Employee e;
    e.id = fields[0];
    e.name = fields[1];
    e.department = fields[2];
    e.designation = fields[3];
    try { e.salary = std::stod(fields[4]); } catch (...) { e.salary = 0; }
    e.joiningDate = fields[5];
    e.phone = fields[6];
    e.email = fields[7];
    try { e.performanceRating = std::stof(fields[8]); } catch (...) { e.performanceRating = 0; }

    return e;
}

bool Employee::isValid() const {
    return !id.empty() && !name.empty() && salary >= 0;
}
