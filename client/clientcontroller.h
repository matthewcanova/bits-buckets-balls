#ifndef CLIENTCONTROLLER_H
#define CLIENTCONTROLLER_H

#include <QObject>
#include <QString>
#include <QMainWindow>
#include <QMessageBox>
#include <startdialog.h>
#include <gameclient.h>
#include <levelview.h>
#include <levelselector.h>
#include <round.h>
#include <continuedialog.h>
#include <gameoverdialog.h>
#include <report.h>
#include <teacherdialog.h>



class clientController : public QObject
{
    Q_OBJECT

public:
    clientController();
    ~clientController();
    LevelView* levelPtr;

signals:
    void sendLevelScores(int*);
    void getTeacherReport();

public slots:
    void updateData(std::string, int*);
    void loginTeacher(QString);
    void startContinueDialog();
    void createGame(int level, int round);
    void continueRound();
    void launchRoundSelector();
    void timedOut();
    void finishRound(int);
    void quitGame();
    void getScoresForLevel(int);
    void generateReport();
    void deleteStudent(QString);

private:
    void calculateLevelandRound();

    StartDialog* startPtr;
    gameClient* clientPtr;
    levelSelector* levelSelectorPtr;
    QMainWindow* currentWindowPtr;
    continueDialog* continueDialogPtr;
    GameOverDialog* gameOverDialogPtr;
    Round* currRoundPtr;
    Report* reportPtr;
    teacherDialog* teacherDialogPtr;
    std::string id, teacherPassword;
    int currentLevel, currentRound;
    int scores[46];
    std::vector<std::vector<int> > reportdata;
    bool existingGame;


};

#endif // CLIENTCONTROLLER_H
