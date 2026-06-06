#include "EmployeeManager.h"
#include "Validation.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <stdexcept>

EmployeeManager::EmployeeManager(const std::string& filePath)
    : dataFilePath(filePath), isDirty(false) {
    loadFromFile();
}

EmployeeManager::~EmployeeManager() {
    if (isDirty) {
        saveToFile();
    }
}

void EmployeeManager::loadFromFile() {
    std::ifstream file(dataFilePath);
    if (!file.is_open()) {
        // File doesn't exist yet — that's fine on first run
        return;
    }

    std::string line;
    int lineNum = 0;
    while (std::getline(file, line)) {
        ++lineNum;
        line = Validation::trim(line);
        if (line.empty() || line[0] == '#') continue;

        Employee emp = Employee::fromCSV(line);
        if (emp.isValid()) {
            employees.push_back(emp);
        } else {
            std::cerr << "[warn] Skipped malformed record at line " << lineNum << "\n";
        }
    }
}

void EmployeeManager::saveToFile() const {
    std::ofstream file(dataFilePath, std::ios::trunc);
    if (!file.is_open()) {
        std::cerr << "[error] Could not write to data file: " << dataFilePath << "\n";
        return;
    }

    file << "# WorkSphere Employee Data — do not edit manually\n";
    for (const auto& emp : employees) {
        file << emp.toCSV() << "\n";
    }
}

void EmployeeManager::flush() {
    saveToFile();
    isDirty = false;
}

bool EmployeeManager::addEmployee(const Employee& emp) {
    if (idExists(emp.getId())) return false;
    employees.push_back(emp);
    isDirty = true;
    saveToFile();
    isDirty = false;
    return true;
}

bool EmployeeManager::updateEmployee(const std::string& id, const Employee& updated) {
    for (auto& emp : employees) {
        if (emp.getId() == id) {
            emp = updated;
            isDirty = true;
            saveToFile();
            isDirty = false;
            return true;
        }
    }
    return false;
}

bool EmployeeManager::deleteEmployee(const std::string& id) {
    auto it = std::find_if(employees.begin(), employees.end(),
        [&id](const Employee& e) { return e.getId() == id; });

    if (it == employees.end()) return false;

    employees.erase(it);
    isDirty = true;
    saveToFile();
    isDirty = false;
    return true;
}

Employee* EmployeeManager::findById(const std::string& id) {
    for (auto& emp : employees) {
        if (emp.getId() == id) return &emp;
    }
    return nullptr;
}

bool EmployeeManager::idExists(const std::string& id) const {
    for (const auto& emp : employees) {
        if (emp.getId() == id) return true;
    }
    return false;
}

std::vector<Employee*> EmployeeManager::searchByName(const std::string& query) {
    std::vector<Employee*> results;
    for (auto& emp : employees) {
        if (Validation::containsIgnoreCase(emp.getName(), query)) {
            results.push_back(&emp);
        }
    }
    return results;
}

std::vector<Employee*> EmployeeManager::searchByDepartment(const std::string& dept) {
    std::vector<Employee*> results;
    for (auto& emp : employees) {
        if (Validation::containsIgnoreCase(emp.getDepartment(), dept)) {
            results.push_back(&emp);
        }
    }
    return results;
}

std::vector<Employee*> EmployeeManager::searchByDesignation(const std::string& desig) {
    std::vector<Employee*> results;
    for (auto& emp : employees) {
        if (Validation::containsIgnoreCase(emp.getDesignation(), desig)) {
            results.push_back(&emp);
        }
    }
    return results;
}

std::vector<Employee> EmployeeManager::getSorted(SortField field, SortOrder order) const {
    std::vector<Employee> sorted = employees;

    auto cmp = [&](const Employee& a, const Employee& b) -> bool {
        bool result = false;
        switch (field) {
            case SortField::NAME:
                result = a.getName() < b.getName();
                break;
            case SortField::SALARY:
                result = a.getSalary() < b.getSalary();
                break;
            case SortField::JOINING_DATE:
                result = a.getJoiningDate() < b.getJoiningDate();
                break;
            case SortField::RATING:
                result = a.getPerformanceRating() < b.getPerformanceRating();
                break;
        }
        return (order == SortOrder::ASC) ? result : !result;
    };

    std::sort(sorted.begin(), sorted.end(), cmp);
    return sorted;
}
