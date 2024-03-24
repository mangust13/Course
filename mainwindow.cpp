#include "mainwindow.h"
#include "./ui_mainwindow.h"
using namespace std;


MainWindow::MainWindow(QWidget *parent):QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    departmentBox = findChild<QComboBox*>("DepartmentBox");
    dateBox = findChild<QComboBox*>("DateBox");

    ui->Table->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->Table, &QTableWidget::customContextMenuRequested, this, &MainWindow::showContextMenu);
}

void MainWindow::showContextMenu(const QPoint &pos) {
    QTableWidget *tableWidget = qobject_cast<QTableWidget*>(sender());
    int selected_row = tableWidget->rowAt(pos.y());
    QMenu contextMenu;
    QAction action1("Видалити", this);
    connect(&action1, &QAction::triggered, this, [=]() { delete_employee(selected_row); });
    contextMenu.addAction(&action1);
    contextMenu.exec(tableWidget->viewport()->mapToGlobal(pos));
}

void MainWindow::delete_employee(int selected_row) {
    if (selected_row >= 0) {
        ui->Table->removeRow(selected_row);
        if (selected_row < employees.size())
        {
            auto it = employees.begin();
            std::advance(it, selected_row);
            employees.erase(it);
        }
    }
}




void MainWindow::on_actionOpen_triggered()
{
    QString appDirPath = QCoreApplication::applicationDirPath();
    QString file_name = QFileDialog::getOpenFileName(this, "Open a file", appDirPath, "Text Files (*.txt)");

    string file_name_std = file_name.toStdString();
    ifstream file(file_name_std);

    if (!file.is_open()) {
        QMessageBox::critical(this, "Помилка", "Не вдалося відкрити файл");
        return;
    }

    employees.clear();
    employees.resize(0);
    on_ClearBut_clicked();

    Employee employee;
    while (file >> employee)
        employees.push_back(employee);

    file.close();
    updateBox();
    fillTable(ui->Table, employees);
}


void MainWindow::on_actionWrite_triggered()
{
    QString appDirPath = QCoreApplication::applicationDirPath();
    QString file_name = QFileDialog::getSaveFileName(this, "Save a file", appDirPath, "Text Files (*.txt)");

    string file_name_std = file_name.toStdString();

    ofstream file(file_name_std);

    if (!file.is_open()) {
        QMessageBox::critical(this, "Помилка", "Не вдалося відкрити файл");
        return;
    }

    for (int row = 0; row < ui->Table->rowCount(); ++row)
    {
        string surname = ui->Table->item(row, 0)->text().toStdString();
        string name = ui->Table->item(row, 1)->text().toStdString();
        string birth = ui->Table->item(row, 2)->text().toStdString();
        string employ = ui->Table->item(row, 3)->text().toStdString();
        string dept = ui->Table->item(row, 4)->text().toStdString();
        string proj = ui->Table->item(row, 5)->text().toStdString();
        double sal = ui->Table->item(row, 6)->text().toDouble();
        Employee employee(surname, name, birth, employ, dept, proj, sal);
        file << employee << "\n";
    }

    file.close();
    QMessageBox::information(this, "", "Файл успішно записано");
}



void MainWindow::on_AddEmployee_clicked()
{
    AddWindow add(this, employees);
    add.setModal(true);
    add.exec();

    updateBox();

    fillTable(ui->Table, employees);
}


void MainWindow::on_SortEmployee_clicked()
{
    try {
        if (employees.empty())
            throw std::runtime_error("Список порожній");

        vector<int> array(employees.size());
        for (const auto& employee : employees)
        {
            int date = stoi(employee.getEmployDate().substr(0,2) + employee.getEmployDate().substr(3,2) + employee.getEmployDate().substr(6,4));
            array.push_back(date);
        }

        auto min = min_element(array.begin(), array.end());
        auto max = max_element(array.begin(), array.end());

        int K = *max - *min + 1;
        vector<int> counts(K, 0);

        for (int i=0; i<K; ++i)
        {
            for (int j=0; j<array.size(); ++j)
            {
                if (array[j] == *min + i)
                   ++counts[i];
            }
        }

        int i = 0;
        for (int j = 0; j < K; j++) {
            while (counts[j] > 0) {
               array[i] = *min + j;
               i++;
               counts[j]--;
            }
        }

        list<Employee> temp;
        for (int i=0; i < array.size(); ++i) {
            for(auto it = employees.begin(); it != employees.end(); ++it)
            {
                int date = stoi(it->getEmployDate().substr(0,2) + it->getEmployDate().substr(3,2) + it->getEmployDate().substr(6,4));
                if (array[i] == date && !it->elementExists(temp))
                    temp.push_back(*it);
            }
        }
        fillTable(ui->Table, temp);
    }
    catch (const std::exception& e) {
        QMessageBox::critical(this, "Помилка", QString::fromStdString(e.what()));
    }
}




void MainWindow::on_ShowYoungOld_clicked()
{
    try {
    if (employees.empty())
        throw std::runtime_error("Список порожній");
    }
    catch (const std::exception& e) {
        QMessageBox::critical(this, "Помилка", QString::fromStdString(e.what()));
    }

    string min_date = "", max_date = "";
    Employee youngest, oldest;
    for (const auto& employee : employees) {
        if (youngest.getBirthDate().empty() || compareDates(employee.getBirthDate(), youngest.getBirthDate())) {
            youngest = employee;
        }

        if (oldest.getBirthDate().empty() || !compareDates(employee.getBirthDate(), oldest.getBirthDate())) {
            oldest = employee;
        }
    }

    list<Employee>temp;
    temp.push_back(youngest);
    temp.push_back(oldest);
    fillTable(ui->Table, temp);
}


void MainWindow::on_ShowDepartm_clicked()
{
    try {
    if (employees.empty())
        throw std::runtime_error("Список порожній");
    }
    catch (const std::exception& e) {
        QMessageBox::critical(this, "Помилка", QString::fromStdString(e.what()));
    }

    string dep = departmentBox->currentText().toStdString();
    int row = 0;
    ui->Table->clear();
    ui->Table->setColumnCount(7);
    ui->Table->setHorizontalHeaderLabels({"Прізвище", "Ім’я", "ДН", "Дата працевлаштування", "Відділ", "Проект", "ЗП"});

    for (const auto& employee: employees) {
        if(employee.getDepartment() == dep){
            ui->Table->insertRow(row);
            ui->Table->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(employee.getSurname())));
            ui->Table->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(employee.getName())));
            ui->Table->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(employee.getBirthDate())));
            ui->Table->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(employee.getEmployDate())));
            ui->Table->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(employee.getDepartment())));
            ui->Table->setItem(row, 5, new QTableWidgetItem(QString::fromStdString(employee.getProject())));
            ui->Table->setItem(row, 6, new QTableWidgetItem(QString::number(employee.getSalary())));
            ++row;
        }
    }
    ui->Table->setRowCount(row);
}


void MainWindow::on_GroupProjects_clicked()
{
    try {
    if (employees.empty())
        throw std::runtime_error("Список порожній");
    }
    catch (const std::exception& e) {
        QMessageBox::critical(this, "Помилка", QString::fromStdString(e.what()));
    }

    // Створення map для зберігання заробітної плати та кількості працівників для кожного проекту
    map<pair<string, string>, pair<double, int>> projectData;

    for (const auto& employee : employees)
    {
        // Перевірка, чи комбінація відділу та проєкту існує в мапі, якщо ні - додати її
        auto key = make_pair(employee.getDepartment(), employee.getProject());
        if (projectData.find(key) == projectData.end())
            projectData[key] = make_pair(0.0, 0);

        // Оновлення кумулятивної заробітної плати та кількості працівників для проекту
        projectData[key].first += employee.getSalary();
        projectData[key].second++;
    }

    list<Employee> projectAverageSalaries;

    for (const auto& entry : projectData)
    {
        double averageSalary = entry.second.first / entry.second.second;

        // Створення об'єкта Employee для проекту з середньою зарплатою
        Employee projectEmployee;
        projectEmployee.setDepartment(entry.first.first);
        projectEmployee.setProject(entry.first.second);
        projectEmployee.setSalary(averageSalary);

        projectAverageSalaries.push_back(projectEmployee);
    }

    ui->Table->clear();
    ui->Table->setColumnCount(3);
    ui->Table->setHorizontalHeaderLabels({"Відділ", "Проєкт","ЗП"});
    int row = 0;

    for (auto it = projectAverageSalaries.begin(); it != projectAverageSalaries.end(); ++it) {
        if (row > 0 && it->getDepartment() != std::prev(it)->getDepartment()) {
            ui->Table->insertRow(row);
            ++row;
        }

        ui->Table->insertRow(row);

        if (row == 0 || it->getDepartment() != std::prev(it)->getDepartment()) {
            ui->Table->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(it->getDepartment())));
        }

        std::string projectName = "    " + it->getProject();
        ui->Table->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(projectName)));
        ui->Table->setItem(row, 2, new QTableWidgetItem(QString::number(it->getSalary())));

        ++row;
    }

    ui->Table->setRowCount(row);
}


void MainWindow::on_ShowSameBut_clicked()
{
    list <Employee> same;

    for (auto it = employees.begin(); it != employees.end(); ++it)
    {
        for (auto it2 = next(it); it2 != employees.end(); ++it2)
        {
            if (it->hasSameBirthAndSalary(*it2))
            {
                if(!it->elementExists(same))
                    same.push_back(*it);
                if(!it2->elementExists(same))
                    same.push_back(*it2);
            }
        }
    }

    if(!same.size())
        QMessageBox::critical(this, "", "Не має працівників із однаковою датою та ЗП");
    else
        fillTable(ui->Table, same);
}


void MainWindow::on_EmployeesInDate_clicked()
{
    string date = dateBox->currentText().toStdString();
    int row = 0;
    ui->Table->clear();
    ui->Table->setColumnCount(7);
    ui->Table->setHorizontalHeaderLabels({"Прізвище", "Ім’я", "ДН", "Дата працевлаштування", "Відділ", "Проект", "ЗП"});

    for(const auto& employee: employees)
    {
        if (employee.getBirthDate() == date)
        {
            ui->Table->insertRow(row);
            ui->Table->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(employee.getSurname())));
            ui->Table->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(employee.getName())));
            ui->Table->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(employee.getBirthDate())));
            ui->Table->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(employee.getEmployDate())));
            ui->Table->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(employee.getDepartment())));
            ui->Table->setItem(row, 5, new QTableWidgetItem(QString::fromStdString(employee.getProject())));
            ui->Table->setItem(row, 6, new QTableWidgetItem(QString::number(employee.getSalary())));
            ++row;
        }
    }
    ui->Table->setRowCount(row);
}


void MainWindow::on_ShowAllBut_clicked()
{
    fillTable(ui->Table, employees);
}


void MainWindow::on_ClearBut_clicked()
{
    ui->Table->clearContents();
    ui->Table->setColumnCount(0);
    ui->Table->setRowCount(0);
    departmentBox->clear();
    dateBox->clear();
}


void MainWindow::fillTable(QTableWidget* table, const list<Employee>& employees) {
    table->clear();
    table->setColumnCount(7);

    table->setHorizontalHeaderLabels({"Прізвище", "Ім’я", "ДН", "Дата працевлаштування", "Відділ", "Проєкт", "ЗП"});

    int row = 0;
    for (const auto& employee : employees) {
        table->insertRow(row);

        table->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(employee.getSurname())));
        table->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(employee.getName())));
        table->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(employee.getBirthDate())));
        table->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(employee.getEmployDate())));
        table->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(employee.getDepartment())));
        table->setItem(row, 5, new QTableWidgetItem(QString::fromStdString(employee.getProject())));
        table->setItem(row, 6, new QTableWidgetItem(QString::number(employee.getSalary())));

        ++row;
    }
    table->setRowCount(row);
}

bool MainWindow::compareDates(const string& date1, const string& date2) {
    tm tm1 = {}, tm2 = {};
    istringstream ss1(date1), ss2(date2);
    ss1 >> get_time(&tm1, "%d.%m.%Y");
    ss2 >> get_time(&tm2, "%d.%m.%Y");

    return mktime(&tm1) < mktime(&tm2);
}

void MainWindow::updateBox()
{
    departmentBox->clear();

    for (const auto& employee : employees)
        uniqueDepartments.insert(employee.getDepartment());

    for (const auto& department : uniqueDepartments)
        departmentBox->addItem(QString::fromStdString(department));

    dateBox->clear();
    set<string> uniqueDates;
    for (const auto& employee: employees)
        uniqueDates.insert(employee.getBirthDate());

    for(const auto& date : uniqueDates)
        dateBox->addItem(QString::fromStdString(date));
}

MainWindow::~MainWindow(){ delete ui;}
