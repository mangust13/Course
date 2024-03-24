#ifndef COMPANYLIST_H
#define COMPANYLIST_H

#include <list>
#include <string>
#include <iostream>
#include <fstream>

class Employee
{
private:
    std::string surname, name, birth_date, employ_date, department, project;
    double salary;
public:
    Employee();
    Employee(std::string surname, std::string name, std::string birth, std::string employ, std::string dept, std::string proj, double sal);
    Employee(const Employee& other);

    bool hasSameBirthAndSalary(const Employee& other);
    bool elementExists(std::list<Employee>& myList);

    std::string getSurname() const { return surname; }
    std::string getName() const { return name; }
    std::string getBirthDate() const { return birth_date; }
    std::string getEmployDate() const { return employ_date; }
    std::string getDepartment() const { return department; }
    std::string getProject() const { return project; }
    double getSalary() const { return salary; }

    void setSurname(const std::string& newSurname) { surname = newSurname; }
    void setName(const std::string& newName) {name = newName;}
    void setBirthDate(const std::string& newBirthDate) { birth_date = newBirthDate; }
    void setEmployDate(const std::string& newEmployDate) { employ_date = newEmployDate; }
    void setDepartment(const std::string& newDepartment) { department = newDepartment; }
    void setProject(const std::string& newProject) { project = newProject; }
    void setSalary(double newSalary) { salary = newSalary;}

    friend std::istream& operator>>(std::istream& is, Employee& employee);
    friend std::ostream& operator<<(std::ostream& os, const Employee& employee);
};

#endif // COMPANYLIST_H
