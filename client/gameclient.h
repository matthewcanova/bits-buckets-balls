#ifndef GAMECLIENT_H
#define GAMECLIENT_H

#include <SFML/Network.hpp>
#include <QString>
#include <QObject>
#include <vector>

#define SERVER_IP "136.60.172.62"
#define SERVER_PORT 53001

class gameClient: public QObject
{
    Q_OBJECT

public:
    gameClient();
    ~gameClient();

    std::string returnCurrentID();
    std::string returnCurrenPassword();
    void sendScore(QString, int, int, int);
    void removeStudent(QString);

public slots:
    void loginStudent(QString, QString, bool);
    void getData();
    void getReport();

signals:
    void updateData(std::string, int*);
    void updateReport(std::vector<std::vector<int> >);
    void chooseRound();
    void bypassContinue();


private:
    sf::TcpSocket socket;
    std::string currentID, currentPassword;
    bool newLogin;
    int scores[46];
    std::vector<std::vector<int> > reportdata;

    void getScores();
    bool successfulLogin();
};

#endif // GAMECLIENT_H
