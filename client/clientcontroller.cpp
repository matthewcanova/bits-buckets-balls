#include "clientcontroller.h"
#include <iostream>
#include <QThread>

clientController::clientController()
{
    startPtr = new StartDialog();
    clientPtr = new gameClient();
    levelPtr = new LevelView();
    levelSelectorPtr = new levelSelector();
    continueDialogPtr = new continueDialog();
    gameOverDialogPtr = new GameOverDialog();
    reportPtr = new Report();
    teacherDialogPtr = new teacherDialog();

    //default values
    currentLevel = 1;
    currentRound = 1;
    existingGame = false;

    //creating a test password for teacher login -- change --
    teacherPassword = "test";

    //setup signals and slots
    connect(startPtr, SIGNAL(loginStudent(QString, QString, bool)), clientPtr, SLOT(loginStudent(QString,QString, bool)));
    connect(startPtr, SIGNAL(loginTeacher(QString)), this, SLOT(loginTeacher(QString)));
    connect(this, SIGNAL(getTeacherReport()), clientPtr, SLOT(getReport()));
    connect(clientPtr, SIGNAL(updateData(std::string, int*)), this, SLOT(updateData(std::string, int*)));
    connect(clientPtr, SIGNAL(updateReport(std::vector<std::vector<int> >)), reportPtr, SLOT(displayReport(std::vector<std::vector<int> >)));
    connect(clientPtr, SIGNAL(chooseRound()), this, SLOT(startContinueDialog()));
    connect(clientPtr, SIGNAL(bypassContinue()), this, SLOT(launchRoundSelector()));
    connect(levelSelectorPtr, SIGNAL(startRound(int,int)), this, SLOT(createGame(int, int)));
    connect(continueDialogPtr, SIGNAL(accepted()), this, SLOT(continueRound()));
    connect(continueDialogPtr, SIGNAL(rejected()), this, SLOT(launchRoundSelector()));
    connect(levelPtr, SIGNAL(finishedRound(int)), this, SLOT(finishRound(int)));
    connect(gameOverDialogPtr, SIGNAL(accepted()), this, SLOT(launchRoundSelector()));
    connect(gameOverDialogPtr, SIGNAL(rejected()), this, SLOT(quitGame()));
    connect(levelPtr, SIGNAL(timedOut()), this, SLOT(timedOut()));
    connect(levelSelectorPtr, SIGNAL(getScores(int)), this, SLOT(getScoresForLevel(int)));
    connect(this, SIGNAL(sendLevelScores(int*)), levelSelectorPtr, SLOT(updateStars(int*)));
    connect(teacherDialogPtr, SIGNAL(generateReport()), this, SLOT(generateReport()));
    connect(teacherDialogPtr, SIGNAL(deleteStudent(QString)), this, SLOT(deleteStudent(QString)));
    connect(teacherDialogPtr, SIGNAL(quitGame()), this, SLOT(quitGame()));

    //show the start dialog
    startPtr->show();
}

clientController::~clientController()
{
    delete startPtr;
    startPtr = nullptr;
    delete clientPtr;
    clientPtr = nullptr;
    delete levelPtr;
    levelPtr = nullptr;
    delete levelSelectorPtr;
    levelSelectorPtr = nullptr;
    delete continueDialogPtr;
    continueDialogPtr = nullptr;
    delete gameOverDialogPtr;
    gameOverDialogPtr = nullptr;
    delete currRoundPtr;
    currRoundPtr = nullptr;
    delete teacherDialogPtr;
    teacherDialogPtr = nullptr;
}

/*User has selected to continue with current progress.
 * Launches the game at the current round*/
void clientController::continueRound()
{
    continueDialogPtr->close();
    calculateLevelandRound();
    createGame(currentRound, currentLevel);
}

/*User has chosen to select the current round.
 * Launches the select round dialog box*/
void clientController::launchRoundSelector()
{
    levelPtr->close();
    startPtr->close();
    gameOverDialogPtr->close();
    continueDialogPtr->close();
    levelSelectorPtr->show();
    getScoresForLevel(currentLevel);
}

/*Creates a new game based on the given round and level*/
void clientController::createGame(int round, int level)
{ 
    levelSelectorPtr->close();

    currentLevel = level;
    currentRound = round;

    currRoundPtr = new Round(level, round);

    Type type = easy;
    //medium rounds
    if (round > 5 && round <= 10 && level != 3)
    {
      type = medium;
    }

    //hard rounds
    else if (round > 10 || (round > 5 && level == 3))
    {
        type = hard;
    }

    //Set appropriate parameters and show
    levelPtr->clear();
    levelPtr->updateLevel(level, type, existingGame);
    int num = currRoundPtr->getNum();
    levelPtr->setAnswer(num);
    levelPtr->startTimer();
    levelPtr->show();
    existingGame = true;
}

/*Updates the current id and scores*/
void clientController::updateData(std::string currentID, int* currentScore)
{
    id = currentID;

    for (int i = 0; i < 46; i++)
    {
        scores[i] = currentScore[i];
    }
}

/* Determines if the password for the teacher login is valid*/
void clientController::loginTeacher(QString password)
{
    if (password.toStdString() == teacherPassword)
    {
        startPtr->close();
        teacherDialogPtr->show();
    }

    else
    {
        QMessageBox msgBox;
        msgBox.setText("Your login was not successful.");
        msgBox.exec();
    }
}


/*Shows a dialog box asking the user if they want to continue at their current level*/
void clientController::startContinueDialog()
{
    startPtr->close();
    continueDialogPtr->show();
}


/*Calculates the level and round based on the vector
 * of scores.  First 0 value in the vector represents the current
 * level and round*/
void clientController::calculateLevelandRound()
{
    //Level 1
    for(int i = 1; i <=15; i++)
    {
        if (scores[i] == 0)
        {
            currentLevel = 1;
            currentRound = i;
            return;
        }
    }

    //Level 2
    for(int i = 16; i <=30; i++)
    {
        if (scores[i] == 0)
        {
            currentLevel = 2;
            currentRound = i - 15;
            return;
        }
    }

    //Level 3
    for(int i = 31; i <=45; i++)
    {
        if (scores[i] == 0)
        {
            currentLevel = 3;
            currentRound = i - 30;
            return;
        }
    }

    //Player has finished all rounds and levels.  Start at the end.
    currentLevel = 3;
    currentRound = 15;
}

/*User has finished a round.  Updates the scores in the database
 * and launches game over dialog box*/
void::clientController::finishRound(int time)
{
    //levelPtr->close();

    //update the score vector and send updated score to database
    int score = currRoundPtr->calculateScore(time);

    int index = (currentLevel - 1) * 15 + currentRound;

    scores[index] = score;
    QString qid = QString::fromStdString(id);
    clientPtr->sendScore(qid, currentLevel, currentRound, score);

    //show game over dialog box
    gameOverDialogPtr->updateScore(score);
    gameOverDialogPtr->show();

    delete currRoundPtr;
    currRoundPtr = nullptr;
}

/*The game has timed out.  Launches a game over dialog box*/
void::clientController::timedOut()
{
    gameOverDialogPtr->updateForTimeout();
    gameOverDialogPtr->show();
}

/*Quits the game with an exit code of 0*/
void::clientController::quitGame()
{
    exit(0);
}

void::clientController::getScoresForLevel(int level)
{
    int levelScores[16];

    for (int i = 1; i <=15; i++)
    {
        if (level == 1)
        {
            levelScores[i] = scores[i];
        }

        else if (level == 2)
        {
            levelScores[i] = scores[i+15];
        }

        else
        {
            levelScores[i] =  scores[i+30];
        }
    }
    emit sendLevelScores(levelScores);
}

/*Displays student progress report for teacher*/
void::clientController::generateReport()
{
    emit getTeacherReport();
}

void::clientController::deleteStudent(QString id)
{
    clientPtr->removeStudent(id);
}


