#include "gameclient.h"
#include <iostream>
#include <QMessageBox>
#include <QTimer>

gameClient::gameClient()
{
    newLogin = false;
}

gameClient::~gameClient()
{
}

/*Sends login information to server.  If login is successful, gets appropriate
 * information from server and sends signals to launch a new game*/
void gameClient::loginStudent(QString id, QString password, bool isNew)
{
    sf::Socket::Status status = socket.connect(SERVER_IP, SERVER_PORT);
    sf::Packet outPacket;
    if (isNew)
    {
        outPacket << "login" << "new" << id.toStdString() << password.toStdString();
    }
    else
    {
        outPacket << "login" << "existing" << id.toStdString() << password.toStdString();
    }
    socket.send(outPacket);
    currentID = id.toStdString();
    currentPassword = password.toStdString();
    if(successfulLogin())
    {
        socket.disconnect();
        getScores();
        emit updateData(currentID, scores);
        if (isNew)
        {
            emit bypassContinue();
        }
        else
        {
            emit chooseRound();
        }
    }
    socket.disconnect();
}

/*Sends information to the server to remove a student from the database*/
void gameClient::removeStudent(QString id) {
    sf::Socket::Status status = socket.connect(SERVER_IP, SERVER_PORT);
    sf::Packet outPacket;
    outPacket << "remove" << id.toStdString();
    socket.send(outPacket);
    getData();
    socket.disconnect();
}

/*Sends updated score information to the server so the database will be updated*/
void gameClient::sendScore(QString id, int level, int round, int score)
{
    sf::Socket::Status status = socket.connect(SERVER_IP, SERVER_PORT);
    sf::Packet outPacket;
    outPacket << "level" << id.toStdString() << level << round << score;
    socket.send(outPacket);
    socket.disconnect();
}

/*Requests the scores from the server and the database of the current student
 * that has logged in successfully.*/
void gameClient::getScores()
{
    sf::Socket::Status status = socket.connect(SERVER_IP, SERVER_PORT);
    sf::Packet outPacket;
    outPacket << "allScores" << currentID;
    socket.send(outPacket);
    getData();
    socket.disconnect();
}

/*Requests information from the server and database to produce the teacher report*/
void gameClient::getReport()
{
    sf::Socket::Status status = socket.connect(SERVER_IP, SERVER_PORT);
    sf::Packet outPacket;
    outPacket << "report";
    socket.send(outPacket);
    getData();
    socket.disconnect();
    emit updateReport(reportdata);
}

/*Receives information from the server and database.  Appropriately parses the data
 * depending upon the first message string in the data packet received*/
void gameClient::getData()
{
    sf::Packet inPacket;
    if(socket.receive(inPacket) == sf::Socket::Done)
    {
        std::string response;
        char* score;
        inPacket >> response;
        if (response == "allscores")
        {
            for (int i = 1; i <=45; i++)
            {
                inPacket >> scores[i];
            }
        }
        else if(response == "report") {
            reportdata.clear();
            int reportsize = 0;
            inPacket >> reportsize; // number of users/players
            for(int i=0; i<reportsize; i++) {
                std::vector<int> userdata;
                int score;
                for(int j=0; j<46; j++) {
                    inPacket >> score;
                    userdata.push_back(score);
                }
                reportdata.push_back(userdata);
            }
        }
        else if(response == "remove") {
            std::string status;
            inPacket >> status;
            QMessageBox msgBox;
            msgBox.setStyleSheet("QMessageBox { background-color: rgb(246, 222, 255) }"
                                  "QPushButton { background-color: rgb(131, 115, 255);");

            if(status == "OK") {
                msgBox.setText(QString("Student removal successful"));
            } else {
                msgBox.setText(QString("Student removal failed"));
            }
             msgBox.exec();
        }
        else
        {
            QMessageBox msgBox;
            msgBox.setStyleSheet("QMessageBox { background-color: rgb(246, 222, 255) }"
                                  "QPushButton { background-color: rgb(131, 115, 255);");
            msgBox.setText(QString::fromUtf8(response.c_str()));
            msgBox.exec();

            return;
        }
    }

    QTimer::singleShot(100, this, SLOT(getData()));
}


/*Returns true if the client could login in successfully to the server and database,
 * false otherwise.  Also displays message if login is not successful.*/
bool gameClient::successfulLogin()
{
    sf::Packet inPacket;
    if(socket.receive(inPacket) == sf::Socket::Done)
    {
        std::string response;
        inPacket >> response;

        if (response == "success")
        {
            return true;
        }
        else
        {
            QMessageBox msgBox;
            msgBox.setStyleSheet("QMessageBox { background-color: rgb(246, 222, 255) }"
                                  "QPushButton { background-color: rgb(131, 115, 255);");
            msgBox.setText("Your login was not successful.");
            msgBox.exec();
            return false;
        }
    }

    QTimer::singleShot(100, this, SLOT(getData()));
}




