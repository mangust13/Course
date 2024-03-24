#include "addwindow.h"
#include "qmessagebox.h"
#include "ui_addwindow.h"

AddWindow::AddWindow(QWidget *parent, std::list<Employee>& mainList) : QDialog(parent), ui(new Ui::AddWindow), dataReference(mainList) {
    ui->setupUi(this);
}

AddWindow::~AddWindow()
{
    delete ui;
}

void AddWindow::on_AddEmployee_clicked()
{
    try{
        std::string surname, name, birth, employ, dept, proj;
        double sal = 0;

        surname = ui->SurnameInput->text().toStdString();
        name = ui->NameInput->text().toStdString();
        birth = ui->BirthdayInput->text().toStdString();
        employ = ui->EmplDate->text().toStdString();
        dept = ui->DepInput->text().toStdString();
        proj = ui->ProjInput->text().toStdString();
        sal = ui->SalaryInput->text().toDouble();

        if (surname.empty() || name.empty() || birth.empty() || employ.empty() || dept.empty() || proj.empty()) {
            throw std::runtime_error("Всі поля повинні бути заповнені");
        }

        if (sal <=0){
            throw std::runtime_error("ЗП не може бути від'ємною");
        }

        bool employeeExists = false;
        for(const auto& employee: dataReference)
        {
            if (employee.getName() == name && employee.getSurname() == surname)
            {
                employeeExists = true;
                break;
            }
        }

        if (!employeeExists)
            dataReference.push_back(Employee(surname, name, birth, employ, dept, proj, sal));
        else
            throw std::runtime_error("Такий працівник уже існує");
    }
    catch(const std::exception& e)
    {
         QMessageBox::critical(this, "Помилка", QString::fromStdString(e.what()));
    }
}
