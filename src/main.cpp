#include <iostream>
#include <string>
#include <limits>
#include "Employee.h"
#include "EmployeeManager.h"
#include "Validation.h"
#include "Display.h"

// ---------------------------------------------------------------
// Input helpers
// ---------------------------------------------------------------

static std::string getInput(const std::string& prompt) {
    std::cout << "  " << prompt;
    std::string line;
    std::getline(std::cin, line);
    return Validation::trim(line);
}

static int getMenuChoice() {
    std::string raw;
    if (!std::getline(std::cin, raw)) {
        return 0;
    }
    raw = Validation::trim(raw);
    try {
        return std::stoi(raw);
    } catch (...) {
        return -1;
    }
}


// ---------------------------------------------------------------
// Feature: Add Employee
// ---------------------------------------------------------------

static void handleAdd(EmployeeManager& mgr) {
    Display::printHeader("Add New Employee");
    std::cout << "  (Type 'cancel' at any prompt to abort)\n\n";

    // ID
    std::string id;
    while (true) {
        id = getInput("Employee ID (e.g. EMP-001): ");
        if (id == "cancel") { Display::printInfo("Aborted."); return; }
        if (!Validation::isValidId(id)) {
            Display::printError("ID must be 3-12 alphanumeric chars (hyphens/underscores OK).");
            continue;
        }
        if (mgr.idExists(id)) {
            Display::printError("An employee with that ID already exists.");
            continue;
        }
        break;
    }

    // Name
    std::string name;
    while (true) {
        name = getInput("Full Name: ");
        if (name == "cancel") { Display::printInfo("Aborted."); return; }
        if (Validation::isValidName(name)) break;
        Display::printError("Name should contain only letters, spaces, hyphens (2-60 chars).");
    }

    std::string dept = getInput("Department: ");
    if (dept == "cancel" || dept.empty()) { Display::printInfo("Aborted."); return; }

    std::string desig = getInput("Designation: ");
    if (desig == "cancel" || desig.empty()) { Display::printInfo("Aborted."); return; }

    // Salary
    double salary = 0;
    while (true) {
        std::string raw = getInput("Salary (USD): ");
        if (raw == "cancel") { Display::printInfo("Aborted."); return; }
        if (Validation::isValidSalary(raw, salary)) break;
        Display::printError("Enter a valid positive number (e.g. 75000).");
    }

    // Date
    std::string date;
    while (true) {
        date = getInput("Joining Date (YYYY-MM-DD): ");
        if (date == "cancel") { Display::printInfo("Aborted."); return; }
        if (Validation::isValidDate(date)) break;
        Display::printError("Date must be in YYYY-MM-DD format.");
    }

    // Phone
    std::string phone;
    while (true) {
        phone = getInput("Phone: ");
        if (phone == "cancel") { Display::printInfo("Aborted."); return; }
        if (Validation::isValidPhone(phone)) break;
        Display::printError("Enter a valid phone number (7-15 digits, may include +, -, spaces).");
    }

    // Email
    std::string email;
    while (true) {
        email = getInput("Email: ");
        if (email == "cancel") { Display::printInfo("Aborted."); return; }
        if (Validation::isValidEmail(email)) break;
        Display::printError("Enter a valid email address.");
    }

    // Rating
    float rating = 0;
    while (true) {
        std::string raw = getInput("Performance Rating (0.0 - 5.0): ");
        if (raw == "cancel") { Display::printInfo("Aborted."); return; }
        if (Validation::isValidRating(raw, rating)) break;
        Display::printError("Rating must be between 0.0 and 5.0.");
    }

    Employee emp(id, name, dept, desig, salary, date, phone, email, rating);

    std::cout << "\n  Confirm adding this employee? (y/n): ";
    std::string confirm;
    std::getline(std::cin, confirm);
    if (Validation::trim(confirm) != "y" && Validation::trim(confirm) != "Y") {
        Display::printInfo("Add cancelled.");
        return;
    }

    if (mgr.addEmployee(emp)) {
        Display::printSuccess("Employee '" + name + "' added successfully.");
    } else {
        Display::printError("Failed to add employee. ID may already exist.");
    }
}

// ---------------------------------------------------------------
// Feature: Update Employee
// ---------------------------------------------------------------

static void handleUpdate(EmployeeManager& mgr) {
    Display::printHeader("Update Employee");

    std::string id = getInput("Enter Employee ID to update: ");
    if (id.empty()) return;

    Employee* existing = mgr.findById(id);
    if (!existing) {
        Display::printError("No employee found with ID: " + id);
        return;
    }

    std::cout << "\n  Current record:\n";
    Display::printEmployee(*existing);
    std::cout << "\n  Enter new values (press Enter to keep current):\n\n";

    auto keepOrNew = [](const std::string& prompt_label, const std::string& current) -> std::string {
        std::cout << "  " << prompt_label << " [" << current << "]: ";
        std::string val;
        std::getline(std::cin, val);
        val = Validation::trim(val);
        return val.empty() ? current : val;
    };

    std::string name = keepOrNew("Full Name", existing->getName());
    std::string dept = keepOrNew("Department", existing->getDepartment());
    std::string desig = keepOrNew("Designation", existing->getDesignation());

    double salary = existing->getSalary();
    while (true) {
        std::cout << "  Salary [" << existing->getSalary() << "]: ";
        std::string raw; std::getline(std::cin, raw);
        raw = Validation::trim(raw);
        if (raw.empty()) break;
        if (Validation::isValidSalary(raw, salary)) break;
        Display::printError("Invalid salary value.");
    }

    std::string date = keepOrNew("Joining Date (YYYY-MM-DD)", existing->getJoiningDate());
    if (!Validation::isValidDate(date)) {
        Display::printError("Invalid date — keeping original.");
        date = existing->getJoiningDate();
    }

    std::string phone = keepOrNew("Phone", existing->getPhone());
    std::string email = keepOrNew("Email", existing->getEmail());

    float rating = existing->getPerformanceRating();
    while (true) {
        std::cout << "  Rating (0-5) [" << existing->getPerformanceRating() << "]: ";
        std::string raw; std::getline(std::cin, raw);
        raw = Validation::trim(raw);
        if (raw.empty()) break;
        if (Validation::isValidRating(raw, rating)) break;
        Display::printError("Rating must be 0.0 - 5.0.");
    }

    Employee updated(id, name, dept, desig, salary, date, phone, email, rating);

    if (mgr.updateEmployee(id, updated)) {
        Display::printSuccess("Record updated.");
    } else {
        Display::printError("Update failed.");
    }
}

// ---------------------------------------------------------------
// Feature: Delete Employee
// ---------------------------------------------------------------

static void handleDelete(EmployeeManager& mgr) {
    Display::printHeader("Delete Employee");

    std::string id = getInput("Enter Employee ID to delete: ");
    if (id.empty()) return;

    Employee* emp = mgr.findById(id);
    if (!emp) {
        Display::printError("No employee found with ID: " + id);
        return;
    }

    Display::printEmployee(*emp);
    std::cout << "\n  Are you sure you want to delete this record? (y/n): ";
    std::string confirm;
    std::getline(std::cin, confirm);

    if (Validation::trim(confirm) != "y" && Validation::trim(confirm) != "Y") {
        Display::printInfo("Deletion cancelled.");
        return;
    }

    if (mgr.deleteEmployee(id)) {
        Display::printSuccess("Employee record deleted.");
    } else {
        Display::printError("Deletion failed.");
    }
}

// ---------------------------------------------------------------
// Feature: Search
// ---------------------------------------------------------------

static void handleSearch(EmployeeManager& mgr) {
    Display::printHeader("Search Employees");
    Display::printSearchMenu();

    int choice = getMenuChoice();
    std::cout << "\n";

    switch (choice) {
        case 1: {
            std::string id = getInput("Search by ID: ");
            Employee* emp = mgr.findById(id);
            if (emp) Display::printEmployee(*emp);
            else Display::printInfo("No employee found with ID: " + id);
            break;
        }
        case 2: {
            std::string query = getInput("Name contains: ");
            auto results = mgr.searchByName(query);
            Display::printEmployeeTablePtr(results);
            break;
        }
        case 3: {
            std::string query = getInput("Department contains: ");
            auto results = mgr.searchByDepartment(query);
            Display::printEmployeeTablePtr(results);
            break;
        }
        case 4: {
            std::string query = getInput("Designation contains: ");
            auto results = mgr.searchByDesignation(query);
            Display::printEmployeeTablePtr(results);
            break;
        }
        case 0:
            return;
        default:
            Display::printError("Invalid option.");
    }
}

// ---------------------------------------------------------------
// Feature: Sort
// ---------------------------------------------------------------

static void handleSort(EmployeeManager& mgr) {
    Display::printHeader("Sort Employees");
    Display::printSortMenu();

    int field = getMenuChoice();
    if (field == 0 || field < 1 || field > 4) return;

    std::cout << "\n  Order: (1) Ascending  (2) Descending — ";
    int orderChoice = getMenuChoice();
    SortOrder order = (orderChoice == 2) ? SortOrder::DESC : SortOrder::ASC;

    SortField sf;
    switch (field) {
        case 1: sf = SortField::NAME; break;
        case 2: sf = SortField::SALARY; break;
        case 3: sf = SortField::JOINING_DATE; break;
        case 4: sf = SortField::RATING; break;
        default: return;
    }

    auto sorted = mgr.getSorted(sf, order);
    Display::printEmployeeTable(sorted);
}

// ---------------------------------------------------------------
// Feature: View Detail
// ---------------------------------------------------------------

static void handleViewDetail(EmployeeManager& mgr) {
    Display::printHeader("Employee Detail");

    std::string id = getInput("Enter Employee ID: ");
    if (id.empty()) return;

    Employee* emp = mgr.findById(id);
    if (!emp) {
        Display::printError("No employee found with ID: " + id);
        return;
    }

    Display::printEmployee(*emp);
}

// ---------------------------------------------------------------
// Main
// ---------------------------------------------------------------

int main() {
    const std::string DATA_PATH = "data/employees.dat";

    EmployeeManager mgr(DATA_PATH);

    bool running = true;
    while (running) {
        Display::printMainMenu();
        int choice = getMenuChoice();

        switch (choice) {
            case 1:
                handleAdd(mgr);
                break;
            case 2:
                Display::printHeader("All Employees");
                if (mgr.count() == 0) {
                    Display::printInfo("No employee records found.");
                } else {
                    Display::printEmployeeTable(mgr.getAll());
                }
                break;
            case 3:
                handleSearch(mgr);
                break;
            case 4:
                handleUpdate(mgr);
                break;
            case 5:
                handleDelete(mgr);
                break;
            case 6:
                handleSort(mgr);
                break;
            case 7:
                handleViewDetail(mgr);
                break;
            case 0:
                Display::printInfo("Goodbye.");
                running = false;
                break;
            default:
                Display::printError("Invalid option. Choose 0-7.");
                continue;
        }

        if (running && choice != 0) {
            Display::pausePrompt();
        }
    }

    return 0;
}
