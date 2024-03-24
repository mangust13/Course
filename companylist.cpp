#include "companylist.h"

Employee::Employee() : surname(""), name(""), birth_date(""), employ_date(""), department(""), project(""), salary(0.0) {}

Employee::Employee(std::string surname, std::string name, std::string birth, std::string employ, std::string dept, std::string proj, double sal)
    : surname(surname), name(name), birth_date(birth), employ_date(employ), department(dept),
    project(proj), salary(sal) {}

Employee::Employee(const Employee& other)
    : surname(other.surname), name(other.name), birth_date(other.birth_date),
    employ_date(other.employ_date), department(other.department),
    project(other.project), salary(other.salary) {}

bool Employee::hasSameBirthAndSalary(const Employee& other)
{
    return (this->birth_date == other.birth_date) && (this->salary == other.salary);
}

bool Employee::elementExists(std::list<Employee>& myList)
{
    for (auto it = myList.begin(); it != myList.end(); ++it)
    {
        if (this->birth_date == it->birth_date && this->salary == it->salary && this->surname == it->surname)
            return true;
    }
    return false;
}

std::istream& operator>>(std::istream& is, Employee& employee) {
    is >> employee.surname >> employee.name >> employee.birth_date >> employee.employ_date
        >> employee.department >> employee.project >> employee.salary;
    return is;
}

std::ostream& operator<<(std::ostream& os, const Employee& employee) {
    os << employee.surname << " " << employee.name << " " << employee.birth_date << " " << employee.employ_date << " "
       << employee.department << " " << employee.project << " " << employee.salary;
    return os;
}
