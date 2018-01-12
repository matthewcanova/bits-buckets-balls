#include "levelselector.h"
#include "ui_levelselector.h"
#include <iostream>

levelSelector::levelSelector(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::levelSelector)
{
    ui->setupUi(this);
    this->setStyleSheet("background-color: lightblue;");
    initializeStarArrays();

    //set up slots and signals
    connect(ui->level1Radio, SIGNAL(toggled(bool)), this, SLOT(level1Selected(bool)));
    connect(ui->level2Radio, SIGNAL(toggled(bool)), this, SLOT(level2Selected(bool)));
    connect(ui->level3Radio, SIGNAL(toggled(bool)), this, SLOT(level3Selected(bool)));
    connect(ui->round1, SIGNAL(clicked(QString)), this, SLOT(clickedLabel(QString)));
    connect(ui->round2, SIGNAL(clicked(QString)), this, SLOT(clickedLabel(QString)));
    connect(ui->round3, SIGNAL(clicked(QString)), this, SLOT(clickedLabel(QString)));
    connect(ui->round4, SIGNAL(clicked(QString)), this, SLOT(clickedLabel(QString)));
    connect(ui->round5, SIGNAL(clicked(QString)), this, SLOT(clickedLabel(QString)));
    connect(ui->round6, SIGNAL(clicked(QString)), this, SLOT(clickedLabel(QString)));
    connect(ui->round7, SIGNAL(clicked(QString)), this, SLOT(clickedLabel(QString)));
    connect(ui->round8, SIGNAL(clicked(QString)), this, SLOT(clickedLabel(QString)));
    connect(ui->round9, SIGNAL(clicked(QString)), this, SLOT(clickedLabel(QString)));
    connect(ui->round10, SIGNAL(clicked(QString)), this, SLOT(clickedLabel(QString)));
    connect(ui->round11, SIGNAL(clicked(QString)), this, SLOT(clickedLabel(QString)));
    connect(ui->round12, SIGNAL(clicked(QString)), this, SLOT(clickedLabel(QString)));
    connect(ui->round13, SIGNAL(clicked(QString)), this, SLOT(clickedLabel(QString)));
    connect(ui->round14, SIGNAL(clicked(QString)), this, SLOT(clickedLabel(QString)));
    connect(ui->round15, SIGNAL(clicked(QString)), this, SLOT(clickedLabel(QString)));

    //default
    level1 = true;
    level2 = false;
    level3 = false;
    emit getScores(1);
}

levelSelector::~levelSelector()
{
    delete ui;
}

/*Determines which label has been clicked, and emits a signal to start a new round*/
void levelSelector::clickedLabel(QString round)
{
    QString subString = round.mid(5);
    int selectedRound = subString.toInt();
    int level = getLevel();
    emit startRound(selectedRound, level);
}

/*Updates the scores/stars for level 1*/
void levelSelector::level1Selected(bool clicked){
    level1 = clicked;
    if (level1)
        emit getScores(1);
}

/*Updates the scores/stars for level 2*/
void levelSelector::level2Selected(bool clicked){
    level2= clicked;
    if (level2)
        emit getScores(2);
}

/*Updates the scores/stars for level 3*/
void levelSelector::level3Selected(bool clicked){
    level3 = clicked;
    if (level3)
        emit getScores(3);
}

/*Returns the level currently selected*/
int levelSelector::getLevel()
{
    if(level1) return 1;
    if(level2) return 2;
    if(level3) return 3;
}

/*Displays the appropriate stars for each round depending on the current score*/
void levelSelector::updateStars(int* scores)
{
    QPixmap star ("../resources/star2.png");

    for (int i = 1; i <= 15; i++)
    {
        if (scores[i] > 0)
        {
            star1Labels[i]->setPixmap(star);
        }
        else
        {
            star1Labels[i]->clear();
        }

        if (scores[i] > 300)
        {
            star2Labels[i]->setPixmap(star);
        }
        else
        {
            star2Labels[i]->clear();
        }

        if (scores[i] > 600)
        {
            star3Labels[i]->setPixmap(star);
        }
        else
        {
            star3Labels[i]->clear();
        }

        if (scores[i] > 900)
        {
            star4Labels[i]->setPixmap(star);
        }
        else
        {
            star4Labels[i]->clear();
        }
    }
}

/*Add all the stars to arrays for easier access in updateStars() method*/
void levelSelector::initializeStarArrays()
{
    star1Labels[1] = ui->star1a;
    star2Labels[1] = ui->star1b;
    star3Labels[1] = ui->star1c;
    star4Labels[1] = ui->star1d;

    star1Labels[2] = ui->star2a;
    star2Labels[2] = ui->star2b;
    star3Labels[2] = ui->star2c;
    star4Labels[2] = ui->star2d;

    star1Labels[3] = ui->star3a;
    star2Labels[3] = ui->star3b;
    star3Labels[3] = ui->star3c;
    star4Labels[3] = ui->star3d;

    star1Labels[4] = ui->star4a;
    star2Labels[4] = ui->star4b;
    star3Labels[4] = ui->star4c;
    star4Labels[4] = ui->star4d;

    star1Labels[5] = ui->star5a;
    star2Labels[5] = ui->star5b;
    star3Labels[5] = ui->star5c;
    star4Labels[5] = ui->star5d;

    star1Labels[6] = ui->star6a;
    star2Labels[6] = ui->star6b;
    star3Labels[6] = ui->star6c;
    star4Labels[6] = ui->star6d;

    star1Labels[7] = ui->star7a;
    star2Labels[7] = ui->star7b;
    star3Labels[7] = ui->star7c;
    star4Labels[7] = ui->star7d;

    star1Labels[8] = ui->star8a;
    star2Labels[8] = ui->star8b;
    star3Labels[8] = ui->star8c;
    star4Labels[8] = ui->star8d;

    star1Labels[9] = ui->star9a;
    star2Labels[9] = ui->star9b;
    star3Labels[9] = ui->star9c;
    star4Labels[9] = ui->star9d;

    star1Labels[10] = ui->star10a;
    star2Labels[10] = ui->star10b;
    star3Labels[10] = ui->star10c;
    star4Labels[10] = ui->star10d;

    star1Labels[11] = ui->star11a;
    star2Labels[11] = ui->star11b;
    star3Labels[11] = ui->star11c;
    star4Labels[11] = ui->star11d;

    star1Labels[12] = ui->star12a;
    star2Labels[12] = ui->star12b;
    star3Labels[12] = ui->star12c;
    star4Labels[12] = ui->star12d;

    star1Labels[13] = ui->star13a;
    star2Labels[13] = ui->star13b;
    star3Labels[13] = ui->star13c;
    star4Labels[13] = ui->star13d;

    star1Labels[14] = ui->star14a;
    star2Labels[14] = ui->star14b;
    star3Labels[14] = ui->star14c;
    star4Labels[14] = ui->star14d;

    star1Labels[15] = ui->star15a;
    star2Labels[15] = ui->star15b;
    star3Labels[15] = ui->star15c;
    star4Labels[15] = ui->star15d;
}
