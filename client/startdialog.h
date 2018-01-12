#ifndef STARTDIALOG_H
#define STARTDIALOG_H

#include <QMainWindow>
#include <QLabel>
#include <timer.h>

namespace Ui {
class StartDialog;
}

class StartDialog : public QMainWindow
{
    Q_OBJECT

public:
    explicit StartDialog(QWidget *parent = 0);
    ~StartDialog();

signals:
    void loginStudent(QString id, QString password, bool isNew);
    void loginTeacher(QString password);

private slots:
    void TeacherLoginSelected(bool clicked);
    void FirstTimeLoginSelected(bool clicked);
    void StudentLoginSelected(bool clicked);
    void accepted();
    void rejected();
    void updateAnimation(int);

private:
    Ui::StartDialog *ui;
    bool student;
    bool teacher;
    bool firstTime;
    QLabel* balls[15];
    int animationCounter;
    void initializeBallArray();
    Timer* timerPtr;
};

#endif // STARTDIALOG_H
