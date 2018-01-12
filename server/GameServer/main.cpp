#include <QCoreApplication>
#include "server.h"
#include "sqlinterface.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Server server;
    // create SQL Server connection
    SQLInterface sql;

    // pass SQL pointer to the game server
    server.assignSQL(&sql);

    // start listening from clients
    server.startListening();
    return a.exec();
}
