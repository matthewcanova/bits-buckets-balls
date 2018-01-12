#include "teacherdialog.h"
#include "ui_teacherdialog.h"

#include <QInputDialog>
#include <QMessageBox>

teacherDialog::teacherDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::teacherDialog)
{
    ui->setupUi(this);

    connect(ui->reportButton, SIGNAL(clicked()), this, SLOT (report()));
    connect(ui->deleteButton, SIGNAL(clicked()), this, SLOT(remove()));
    connect(ui->exitButton, SIGNAL (clicked()), this, SLOT(exit()));
}

teacherDialog::~teacherDialog()
{
    delete ui;
}

void teacherDialog::report()
{
    emit generateReport();
}

void teacherDialog::remove()
{
    bool ok;
    QString username = QInputDialog::getText(this, "Student ID to delete", "Student ID:", QLineEdit::Normal, "", &ok);
    if(ok && !username.isEmpty()) {
        try {
            std::stoi(username.toStdString());
            emit deleteStudent(username);
        } catch(const std::invalid_argument& ia) {
            QMessageBox errormessage;
            errormessage.setText("Invalid student ID");
            errormessage.exec();
            return;
        }
    }
}

void teacherDialog::exit()
{
    emit quitGame();
}

