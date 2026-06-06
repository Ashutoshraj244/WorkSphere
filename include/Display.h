#pragma once
#include "Employee.h"
#include <vector>
#include <string>

namespace Display {
    void printHeader(const std::string& title);
    void printDivider();
    void printEmployee(const Employee& emp);
    void printEmployeeTable(const std::vector<Employee>& list);
    void printEmployeeTablePtr(const std::vector<Employee*>& list);
    void printSuccess(const std::string& msg);
    void printError(const std::string& msg);
    void printInfo(const std::string& msg);
    void printWarning(const std::string& msg);
    void clearScreen();
    void pausePrompt();

    // Menu helpers
    void printMainMenu();
    void printSearchMenu();
    void printSortMenu();
}
