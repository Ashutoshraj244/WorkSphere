#pragma once
#include <string>

class Employee {
private:
    std::string id;
    std::string name;
    std::string department;
    std::string designation;
    double salary;
    std::string joiningDate;
    std::string phone;
    std::string email;
    float performanceRating;

public:
    Employee() : salary(0.0), performanceRating(0.0) {}

    Employee(const std::string& id, const std::string& name, const std::string& dept,
             const std::string& desig, double sal, const std::string& date,
             const std::string& phone, const std::string& email, float rating)
        : id(id), name(name), department(dept), designation(desig),
          salary(sal), joiningDate(date), phone(phone), email(email),
          performanceRating(rating) {}

    // Getters
    std::string getId() const { return id; }
    std::string getName() const { return name; }
    std::string getDepartment() const { return department; }
    std::string getDesignation() const { return designation; }
    double getSalary() const { return salary; }
    std::string getJoiningDate() const { return joiningDate; }
    std::string getPhone() const { return phone; }
    std::string getEmail() const { return email; }
    float getPerformanceRating() const { return performanceRating; }

    // Setters
    void setName(const std::string& n) { name = n; }
    void setDepartment(const std::string& d) { department = d; }
    void setDesignation(const std::string& d) { designation = d; }
    void setSalary(double s) { salary = s; }
    void setJoiningDate(const std::string& d) { joiningDate = d; }
    void setPhone(const std::string& p) { phone = p; }
    void setEmail(const std::string& e) { email = e; }
    void setPerformanceRating(float r) { performanceRating = r; }

    // Serialize to CSV line
    std::string toCSV() const;

    // Deserialize from CSV line
    static Employee fromCSV(const std::string& line);

    bool isValid() const;
};
