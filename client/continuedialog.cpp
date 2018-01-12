#include "continuedialog.h"
#include "ui_continuedialog.h"
#include <iostream>

continueDialog::continueDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::continueDialog)
{
    ui->setupUi(this);

    connect(ui->yesButton, SIGNAL(clicked()), this, SLOT(yes_pushed()));
    connect(ui->noButton, SIGNAL(clicked()), this, SLOT(no_pushed()));
}

continueDialog::~continueDialog()
{
    delete ui;
}

void continueDialog:: yes_pushed()
{
    emit accepted();
}

void continueDialog::no_pushed()
{
    emit rejected();
}
