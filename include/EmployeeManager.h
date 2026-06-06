#pragma once
#include "Employee.h"
#include <vector>
#include <string>
#include <functional>

enum class SortField { NAME, SALARY, JOINING_DATE, RATING };
enum class SortOrder { ASC, DESC };

class EmployeeManager {
private:
    std::vector<Employee> employees;
    std::string dataFilePath;
    bool isDirty; // tracks unsaved changes

    void loadFromFile();
    void saveToFile() const;

public:
    explicit EmployeeManager(const std::string& filePath);
    ~EmployeeManager();

    // CRUD
    bool addEmployee(const Employee& emp);
    bool updateEmployee(const std::string& id, const Employee& updated);
    bool deleteEmployee(const std::string& id);
    Employee* findById(const std::string& id);

    // Search
    std::vector<Employee*> searchByName(const std::string& query);
    std::vector<Employee*> searchByDepartment(const std::string& dept);
    std::vector<Employee*> searchByDesignation(const std::string& desig);

    // Sorting
    std::vector<Employee> getSorted(SortField field, SortOrder order = SortOrder::ASC) const;

    // Accessors
    const std::vector<Employee>& getAll() const { return employees; }
    bool idExists(const std::string& id) const;
    int count() const { return static_cast<int>(employees.size()); }

    void flush(); // force save
};
