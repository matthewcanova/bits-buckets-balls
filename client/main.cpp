#include "startdialog.h"
#include "clientcontroller.h"
#include <QApplication>
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    clientController c;

    return a.exec();
}
