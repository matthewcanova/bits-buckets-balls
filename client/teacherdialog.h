#ifndef TEACHERDIALOG_H
#define TEACHERDIALOG_H

#include <QDialog>


namespace Ui {
class teacherDialog;
}

class teacherDialog :public QDialog
{
    Q_OBJECT

public:
    explicit teacherDialog(QWidget *parent = 0);
    ~teacherDialog();

signals:
    void generateReport();
    void deleteStudent(QString);
    void quitGame();

public slots:
    void report();
    void remove();
    void exit();

private:
    Ui::teacherDialog *ui;
};

#endif // TEACHERDIALOG_H

