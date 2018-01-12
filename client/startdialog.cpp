#include "startdialog.h"
#include "ui_startdialog.h"
#include <iostream>

StartDialog::StartDialog(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StartDialog)
{
    ui->setupUi(this);

    //put balls into array for animation
    initializeBallArray();

    ui->passwordEntryConfirm->hide();
    ui->passwordLabelConfirm->hide();
    ui->passwordWarning->hide();
    ui->numberOnlyWarning->hide();
    ui->tooLongWarning->hide();
    ui->emptyFieldsWarning->hide();

    //Setup timer for animation
    timerPtr = new Timer();
    timerPtr->startTime();
    animationCounter = 1;


    // Setup signals & slots
    connect(ui->teacherLogin, SIGNAL(toggled(bool)), this, SLOT(TeacherLoginSelected(bool)));
    connect(ui->studentLogin, SIGNAL(toggled(bool)), this, SLOT(StudentLoginSelected(bool)));
    connect(ui->firstTimeLogin, SIGNAL(toggled(bool)), this, SLOT(FirstTimeLoginSelected(bool)));
    connect(ui->enterButton, SIGNAL(clicked()), this, SLOT(accepted()));
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(rejected()));
    connect(timerPtr, SIGNAL(updateTime(int)),this, SLOT(updateAnimation(int)));


    //Start out with student login selected
    student = true;
    teacher = false;
    firstTime = false;
    StudentLoginSelected(true);
}

StartDialog::~StartDialog()
{
    delete ui;
}

void StartDialog::StudentLoginSelected(bool clicked)
{
    student = clicked;

    if (clicked)
    {
        // Show id entries
        ui->idEntry->show();
        ui->idLabel->show();
        ui->idEntry->setFocus();

        // Hide second password entries
        ui->passwordEntryConfirm->hide();
        ui->passwordLabelConfirm->hide();
        //ui->buttonBox->setGeometry(ui->buttonBox->x(), 200, ui->buttonBox->width(), ui->buttonBox->height());

        // Hide any warning messages
        ui->passwordWarning->hide();
        ui->emptyFieldsWarning->hide();
        ui->numberOnlyWarning->hide();
        ui->tooLongWarning->hide();
    }
}

/* Changes ID label to Teacher Id instead of Student Id.*/
void StartDialog::TeacherLoginSelected(bool clicked)
{
    teacher = clicked;

    if (clicked)
    {
        // Hide id label and entry
        ui->idLabel->hide();
        ui->idEntry->hide();

        // Hide second password entries
        ui->passwordEntryConfirm->hide();
        ui->passwordLabelConfirm->hide();
        //ui->buttonBox->setGeometry(ui->buttonBox->x(), 200, ui->buttonBox->width(), ui->buttonBox->height());

        // Hide any warning messages
        ui->passwordWarning->hide();
        ui->emptyFieldsWarning->hide();
        ui->numberOnlyWarning->hide();
        ui->tooLongWarning->hide();

        ui->passwordEntry->setFocus();
    }
}

/* Displays another password entry box for confirmation. Changes OK button display to Sign-Up.*/
void StartDialog::FirstTimeLoginSelected(bool clicked)
{
    firstTime = clicked;

    if (clicked)
    {
        // Show id entries
        ui->idEntry->show();
        ui->idLabel->show();
        ui->idEntry->setFocus();

        // Show second password entries
        ui->passwordEntryConfirm->show();
        ui->passwordLabelConfirm->show();
        //ui->buttonBox->setGeometry(ui->buttonBox->x(), 220, ui->buttonBox->width(), ui->buttonBox->height());

        // Hide any warning messages
        ui->passwordWarning->hide();
        ui->emptyFieldsWarning->hide();
        ui->numberOnlyWarning->hide();
        ui->tooLongWarning->hide();
    }
}

/* Checks to see which radio button selected and attempts to connect to server appropriately.*/
void StartDialog::accepted()
{
    // Hide any warning messages
    ui->passwordWarning->hide();
    ui->emptyFieldsWarning->hide();
    ui->numberOnlyWarning->hide();
    ui->tooLongWarning->hide();

    if (firstTime)
    {

        QString userName = ui->idEntry->text();

        // Check that login is not empty and is composed of only integers
        if (userName == nullptr || userName == "")
        {
            ui->emptyFieldsWarning->show();
            return;
        }
        std::string id = userName.toStdString();
        for(char c : id)
        {
            std::string currChar;
            currChar += c;

            try
            {
                std::stoi(currChar);
            }
            catch(const std::invalid_argument& ia)
            {
                ui->numberOnlyWarning->show();
                return;
            }
            catch(const std::out_of_range& oor)
            {
                ui->tooLongWarning->show();
                return;
            }
        }

        // Check that passwords are not empty and match
        QString password1 = ui->passwordEntry->text();
        QString password2 = ui->passwordEntryConfirm->text();
        if (password1 == nullptr || password1 == "" || password2 == nullptr || password2 == "")
        {
            ui->emptyFieldsWarning->show();
            return;
        }

        if (password1 != password2)
        {
            ui->passwordWarning->show();
            return;
        }
        else
        {
            // Connect to server
            emit loginStudent(ui->idEntry->text(), ui->passwordEntry->text(), true);
        }
    }
    else if (student)
    {
        // Check that password and id input are not empty & id is integer
        QString userName = ui->idEntry->text();
        QString password = ui->passwordEntry->text();

        if (userName == nullptr || userName == "" || password == nullptr || password == "")
        {
            ui->emptyFieldsWarning->show();
            return;
        }

        std::string id = userName.toStdString();
        for(char c : id)
        {
            std::string currChar;
            currChar += c;

            try
            {
                std::stoi(currChar);
            }
            catch(const std::invalid_argument& ia)
            {
                ui->numberOnlyWarning->show();
                return;
            }
            catch(const std::out_of_range& oor)
            {
                ui->tooLongWarning->show();
                return;
            }
        }

        emit loginStudent(userName, password, false);
    }

    else if (teacher)
    {
        // Check that password is not empty
        QString password = ui->passwordEntry->text();
        if (password == nullptr || password == "")
        {
            ui->emptyFieldsWarning->show();
            return;
        }

        emit loginTeacher(ui->passwordEntry->text());
    }
}

/* Close window if Cancel pressed.*/
void StartDialog::rejected()
{
    this->close();
    exit(0);
}

void::StartDialog::initializeBallArray()
{
    balls[1] = ui->ball1;
    balls[2] = ui->ball2;
    balls[3] = ui->ball3;
    balls[4] = ui->ball4;
    balls[5] = ui->ball5;
    balls[6] = ui->ball6;
    balls[7] = ui->ball7;
    balls[8] = ui->ball8;
    balls[9] = ui->ball9;
    balls[10] = ui->ball10;
    balls[11] = ui->ball11;
    balls[12] = ui->ball12;
    balls[13] = ui->ball13;
    balls[14] = ui->ball14;

    for (int i = 2; i <=14; i++)
    {
        balls[i]->hide();
    }

    balls[10]->lower();
    balls[11]->lower();
    balls[12]->lower();
    balls[13]->lower();
    balls[14]->lower();

}

void::StartDialog::updateAnimation(int time)
{
    if (animationCounter < 14)
    {
        balls[animationCounter]->hide();
        balls[++animationCounter]->show();
    }
}
