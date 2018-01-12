#ifndef MAIN_H
#define MAIN_H

#include <QtCore>
#include <SFML/Network.hpp>
#include <vector>
#include <string>

#include "sqlinterface.h"

// TCP port the server should listen on
#define PORT 53001

class Server : public QObject {
    Q_OBJECT

public:
    explicit Server(QObject *parent = 0);
    ~Server();
    void assignSQL(SQLInterface*);
    int startListening();

private slots:
    void listenForConnections();
    void getData();

private:
    bool isUsernameValid(std::string);
    std::vector<sf::TcpSocket*> clientlist;
    sf::Socket::Status listener_status;
    sf::TcpListener listener;

    // SQL interface pointer
    SQLInterface *sqlinterface;
};

#endif // MAIN_H
