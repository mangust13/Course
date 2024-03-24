#ifndef ADDWINDOW_H
#define ADDWINDOW_H

#include <QDialog>
#include "companylist.h"

namespace Ui {class AddWindow;}

class AddWindow : public QDialog
{
    Q_OBJECT
public:
    explicit AddWindow(QWidget *parent, std::list<Employee>& mainList);
    ~AddWindow();
private slots:
    void on_AddEmployee_clicked();
private:
    Ui::AddWindow *ui;
    std::list <Employee>& dataReference;
};

#endif // ADDWINDOW_H
