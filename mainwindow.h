#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "addwindow.h"
#include "companylist.h"
#include "qtablewidget.h"
#include <list>
#include <QMessageBox>
#include <QFileDialog>
#include <fstream>
#include <vector>
#include <QComboBox>
#include <algorithm>
#include <sstream>
#include <set>

using namespace std;
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void showContextMenu(const QPoint &pos);
    void fillTable(QTableWidget * table, const std::list<Employee>& employees);
    bool compareDates(const string& date1, const string& date2);
    list <Employee> employees;
    void delete_employee(int selected_row);
    void updateBox();
private slots:
    void on_AddEmployee_clicked();
    void on_actionOpen_triggered();
    void on_SortEmployee_clicked();
    void on_ShowYoungOld_clicked();
    void on_ShowDepartm_clicked();
    void on_ShowAllBut_clicked();
    void on_ShowSameBut_clicked();
    void on_EmployeesInDate_clicked();
    void on_ClearBut_clicked();
    void on_GroupProjects_clicked();
    void on_actionWrite_triggered();

private:
    Ui::MainWindow *ui;
    QComboBox *departmentBox, *dateBox;
    set<string> uniqueDepartments;  // Використання множини для забезпечення унікальності
};
#endif // MAINWINDOW_H
