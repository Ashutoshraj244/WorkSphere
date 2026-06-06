#include "Display.h"
#include <iostream>
#include <iomanip>
#include <string>

namespace Display {

static const int COL_ID    = 10;
static const int COL_NAME  = 22;
static const int COL_DEPT  = 16;
static const int COL_DESIG = 20;
static const int COL_SAL   = 12;
static const int COL_DATE  = 12;
static const int COL_RATE  = 7;

void printDivider() {
    std::cout << std::string(105, '-') << "\n";
}

void printHeader(const std::string& title) {
    std::cout << "\n";
    printDivider();
    int pad = (105 - (int)title.size()) / 2;
    std::cout << std::string(pad, ' ') << title << "\n";
    printDivider();
}

void printSuccess(const std::string& msg) {
    std::cout << "  [OK] " << msg << "\n";
}

void printError(const std::string& msg) {
    std::cout << "  [ERR] " << msg << "\n";
}

void printInfo(const std::string& msg) {
    std::cout << "  [--] " << msg << "\n";
}

void printWarning(const std::string& msg) {
    std::cout << "  [!!] " << msg << "\n";
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pausePrompt() {
    std::cout << "\n  Press Enter to continue...";
    std::cin.ignore(10000, '\n');
    std::cin.get();
}

static void printTableHeader() {
    printDivider();
    std::cout << std::left
              << std::setw(COL_ID)    << "  ID"
              << std::setw(COL_NAME)  << "Name"
              << std::setw(COL_DEPT)  << "Department"
              << std::setw(COL_DESIG) << "Designation"
              << std::setw(COL_SAL)   << "Salary"
              << std::setw(COL_DATE)  << "Joined"
              << std::setw(COL_RATE)  << "Rating"
              << "\n";
    printDivider();
}

static void printTableRow(const Employee& emp) {
    std::string name = emp.getName();
    if (name.size() > COL_NAME - 2) name = name.substr(0, COL_NAME - 5) + "...";
    std::string desig = emp.getDesignation();
    if (desig.size() > COL_DESIG - 2) desig = desig.substr(0, COL_DESIG - 5) + "...";

    std::cout << std::left
              << "  " << std::setw(COL_ID - 2) << emp.getId()
              << std::setw(COL_NAME)  << name
              << std::setw(COL_DEPT)  << emp.getDepartment()
              << std::setw(COL_DESIG) << desig
              << "$" << std::setw(COL_SAL - 1) << std::fixed << std::setprecision(0) << emp.getSalary()
              << std::setw(COL_DATE)  << emp.getJoiningDate()
              << std::setw(COL_RATE)  << std::setprecision(1) << emp.getPerformanceRating()
              << "\n";
}

void printEmployeeTable(const std::vector<Employee>& list) {
    if (list.empty()) {
        printInfo("No records to display.");
        return;
    }
    printTableHeader();
    for (const auto& emp : list) {
        printTableRow(emp);
    }
    printDivider();
    std::cout << "  Total: " << list.size() << " record(s)\n";
}

void printEmployeeTablePtr(const std::vector<Employee*>& list) {
    if (list.empty()) {
        printInfo("No matching records found.");
        return;
    }
    printTableHeader();
    for (const auto* emp : list) {
        printTableRow(*emp);
    }
    printDivider();
    std::cout << "  Total: " << list.size() << " match(es)\n";
}

void printEmployee(const Employee& e) {
    printDivider();
    std::cout << "  Employee ID     : " << e.getId() << "\n";
    std::cout << "  Full Name       : " << e.getName() << "\n";
    std::cout << "  Department      : " << e.getDepartment() << "\n";
    std::cout << "  Designation     : " << e.getDesignation() << "\n";
    std::cout << "  Salary          : $" << std::fixed << std::setprecision(2) << e.getSalary() << "\n";
    std::cout << "  Joining Date    : " << e.getJoiningDate() << "\n";
    std::cout << "  Phone           : " << e.getPhone() << "\n";
    std::cout << "  Email           : " << e.getEmail() << "\n";
    std::cout << "  Rating (0-5)    : " << std::setprecision(1) << e.getPerformanceRating() << "\n";
    printDivider();
}

void printMainMenu() {
    std::cout << "\n";
    printDivider();
    std::cout << "                       WorkSphere  —  Employee Record System\n";
    printDivider();
    std::cout << "\n";
    std::cout << "    1.  Add New Employee\n";
    std::cout << "    2.  View All Employees\n";
    std::cout << "    3.  Search Employees\n";
    std::cout << "    4.  Update Employee\n";
    std::cout << "    5.  Delete Employee\n";
    std::cout << "    6.  Sort & View Employees\n";
    std::cout << "    7.  View Employee Detail\n";
    std::cout << "\n";
    std::cout << "    0.  Exit\n";
    std::cout << "\n";
    printDivider();
    std::cout << "  Select option: ";
}

void printSearchMenu() {
    std::cout << "\n  Search by:\n";
    std::cout << "    1. Employee ID\n";
    std::cout << "    2. Name\n";
    std::cout << "    3. Department\n";
    std::cout << "    4. Designation\n";
    std::cout << "    0. Back\n";
    std::cout << "\n  Select: ";
}

void printSortMenu() {
    std::cout << "\n  Sort by:\n";
    std::cout << "    1. Name\n";
    std::cout << "    2. Salary\n";
    std::cout << "    3. Joining Date\n";
    std::cout << "    4. Performance Rating\n";
    std::cout << "    0. Back\n";
    std::cout << "\n  Select: ";
}

} // namespace Display
