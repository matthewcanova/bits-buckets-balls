#include "levelview.h"
#include "ui_levelView.h"
#include <QPushButton>
#include <QLabel>
#include <QPalette>
#include <QFont>
#include <iostream>
#include <math.h>

LevelView::LevelView(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::LevelView)
{
    ui->setupUi(this);

    //default settings
    level = 1;
    type = easy;
    timerPtr = new Timer();
    //canvas = new myCanvas(this, QPoint(25,100), QSize(1200, 300));


    //initialize values
    for (int i = 0; i < 9; i++)
    {
        values[i] = 0;
        bits[i] = 0;
    }

    //Add labels to array
    bitLabels[1] = ui->place1;
    bitLabels[2] = ui->place2;
    bitLabels[3] = ui->place4;
    bitLabels[4] = ui->place8;
    bitLabels[5] = ui->place16;
    bitLabels[6] = ui->place32;
    bitLabels[7] = ui->place64;
    bitLabels[8] = ui->place128;

    //used for testing -- remove when SFML window is up
    ball1 = false;
    ball2 = false;
    ball4 = false;
    ball8 = false;
    ball16 = false;
    ball32 = false;
    ball64 = false;
    ball128 = false;
    hex1 = false;
    hex2 = false;

    //up slots and signals
    connect(ui->graphicsView, SIGNAL(updateContactToLevelView(int, int, bool)) , this, SLOT(changeBits(int, int, bool)));
    //connect (ui->Ball1, SIGNAL(clicked()), this, SLOT(clickedBits1()));
    //connect (ui->Ball2, SIGNAL(clicked()), this, SLOT(clickedBits2()));
    //connect (ui->Ball4, SIGNAL(clicked()), this, SLOT(clickedBits4()));
    //connect (ui->Ball8, SIGNAL(clicked()), this, SLOT(clickedBits8()));
    //connect (ui->Ball16, SIGNAL(clicked()), this, SLOT(clickedBits16()));
    //connect (ui->Ball32, SIGNAL(clicked()), this, SLOT(clickedBits32()));
    //connect (ui->Ball64, SIGNAL(clicked()), this, SLOT(clickedBits64()));
    //connect (ui->Ball128, SIGNAL(clicked()), this, SLOT(clickedBits128()));
    //connect (ui->HexButton1, SIGNAL(clicked(bool)), this, SLOT(h1clicked()));
    //connect (ui->HexButton2, SIGNAL(clicked(bool)), this, SLOT(h2clicked()));
    connect (this, SIGNAL(clickedBits(int,bool)), this, SLOT(updateBits(int,bool)));
    connect (this, SIGNAL(clickedBits(int, int, bool)), this, SLOT(updateBits(int,int,bool)));
    connect(timerPtr, SIGNAL(updateTime(int)),this, SLOT(updateTime(int)));
    connect(timerPtr, SIGNAL(stopRound()), this, SLOT(stopRound()));
    //connect(canvas->myTimer, SIGNAL(timeout()), canvas, SLOT(onUpdate()));
}


LevelView::~LevelView()
{
    delete ui;
    delete timerPtr;
    timerPtr = nullptr;
}

/*Clears the bit values and sets all bits and values back to 0*/
void::LevelView::clear()
{

    //used for testing -- remove when SFML window is up
    ball1 = false;
    ball2 = false;
    ball4 = false;
    ball8 = false;
    ball16 = false;
    ball32 = false;
    ball64 = false;
    ball128 = false;
    hex1 = false;
    hex2 = false;

    //default settings
    level = 1;
    type = easy;

    updateScore(0);

    //initialize values
    for (int i = 1; i < 9; i++)
    {
        values[i] = 0;
        bits[i] = 0;
    }

    for (int i = 1; i < 9; i++)
    {
        updateBitLabel(i, false);
    }

    QString value = QString::number(0);
    ui->one->setText(value);
    ui->two->setText(value);
    ui->four->setText(value);
    ui->eight->setText(value);
    ui->sixteen->setText(value);
    ui->thirtytwo->setText(value);
    ui->sixtyfour->setText(value);
    ui->onetwentyeight->setText(value);
    ui->answer->setText(value);
}

/* Sets the answer (the number the user is working on) to the number given*/
void LevelView::setAnswer(int num)
{
    answer = num;
    QString text = QString::number(num);
    ui->decimalNumber->setText(text);
}

/* Updates the GUI appropriately for the level and type.
 * Easy level has four bits, medium level has 6 bits and hard level has 8.
 * Level 1 is binary, Level 2 is two's complement, Level 3 is hexadecimal*/
void LevelView::updateLevel(int gameLevel, Type gameType, bool existingGame)
{
    level = gameLevel;
    type = gameType;

    if (type == easy || type == medium)
    {
        //ui->Ball64->hide();
        //ui->Ball128->hide();
        ui->place64->hide();
        ui->place128->hide();
        ui->sixtyfour->hide();
        ui->onetwentyeight->hide();
        ui->plus6->hide();
        ui->plus7->hide();
    }

    if (type == easy)
    {
        //ui->Ball16->hide();
        //ui->Ball32->hide();
        ui->place16->hide();
        ui->place32->hide();
        ui->sixteen->hide();
        ui->thirtytwo->hide();
        ui->plus4->hide();
        ui->plus5->hide();
    }

    if (type == medium || type == hard)
    {
        //ui->Ball16->show();
        //ui->Ball32->show();
        ui->place16->show();
        ui->place32->show();
        ui->sixteen->show();
        ui->thirtytwo->show();
        ui->plus4->show();
        ui->plus5->show();
    }

    if (type == hard)
    {
        //ui->Ball64->show();
        //ui->Ball128->show();
        ui->place64->show();
        ui->place128->show();
        ui->sixtyfour->show();
        ui->onetwentyeight->show();
        ui->plus6->show();
        ui->plus7->show();
    }

    if(level == 2)
        ui->levelLabel->setText("Two's complement");
    if (level == 3)
        ui->levelLabel->setText("Hexadecimal");
    ui->graphicsView->setupLevel(level, type, existingGame);
    //ui->graphicsView->setupLevel(level, type);
}


void LevelView::startTimer()
{
    timerPtr->startTime();
}



void LevelView::updateTime(int time)
{
    int minutes = time / 60000;
    QString min = QString("%1").arg(minutes, 2, 10, QChar('0'));
    int seconds = (time / 1000) % 60;
    QString sec = QString("%1").arg(seconds, 2, 10, QChar('0'));
    ui->seconds->display(sec);
    ui->minutes->display(min);
}

void LevelView::updateBits(int place, bool on)
{
    if (level != 3)
    {
        if (on)
            bits[place] = 1;
        else
        {
            bits[place] = 0;
        }
    }
    updateValues();
}

void LevelView::updateBits(int place, int hexVal, bool on)
{
    if (place == 1)
    {
        std::vector<int> results = convertToBinary(hexVal);
        for (int i = 0, k = 4; i < 4, k > 0; i++, k--)
        {
            if(on)
                bits[k] = results[i];

            else
                bits[k] = 0;

            if(bits[k] == 0) updateBitLabel(k, false);
            else updateBitLabel(k, true);

        }
    }

    else if (place == 2)
    {
        std::vector<int> results = convertToBinary(hexVal);
        for (int i = 0, k = 8; i < 4, k > 4; i++, k--)
        {
            if(on)
                bits[k] = results[i];
            else
               bits[k] = 0;

            if(bits[k] == 0) updateBitLabel(k, false);
            else updateBitLabel(k, true);
        }
    }
    updateValues();
}

void LevelView::updateValues()
{
    int length;
    if (type == easy)
        length = 4;
    else if (type == medium)
        length = 6;
    else
        length = 8;

    for (int i = 1; i <=length; i++)
    {
        values[i] = bits[i] * pow(2, i-1);
    }

    if (level == 2)
    {
        values[length] = bits[length] * -1 * pow(2,length-1);
    }

    QString value;
    value = QString::number(values[1]);
    ui->one->setText(value);
    value = QString::number(values[2]);
    ui->two->setText(value);
    value = QString::number(values[3]);
    ui->four->setText(value);
    value = QString::number(values[4]);
    ui->eight->setText(value);

    if (type != easy)
    {
        value = QString::number(values[5]);
        ui->sixteen->setText(value);
        value = QString::number(values[6]);
        ui->thirtytwo->setText(value);
    }

    if (type == hard)
    {
        value = QString::number(values[7]);
        ui->sixtyfour->setText(value);
        value = QString::number(values[8]);
        ui->onetwentyeight->setText(value);
    }

    //calculate total value and update label
    int total = 0;
    for (int i = 1; i <=8; i++)
    {
        total += values[i];
    }
    QString qtotal = QString::number(total);
    ui->answer->setText(qtotal);

    //check if answer matches the problem number
    if (total == answer && total != 0)
    {
        emit finishedRound(timerPtr->returnCurrentTime());
        timerPtr->stopTime();
    }
}

std::vector<int> LevelView::convertToBinary(int c)
{
    switch(c)
    {
       case 0: return {0,0,0,0};
       case 1: return {0,0,0,1};
       case 2: return {0,0,1,0};
       case 3: return {0,0,1,1};
       case 4: return {0,1,0,0};
       case 5: return {0,1,0,1};
       case 6: return {0,1,1,0};
       case 7: return {0,1,1,1};
       case 8: return {1,0,0,0};
       case 9: return {1,0,0,1};
       case 10: return {1,0,1,0};
       case 11: return {1,0,1,1};
       case 12: return {1,1,0,0};
       case 13: return {1,1,0,1};
       case 14: return {1,1,1,0};
       case 15: return {1,1,1,1};
    default:
        return {0,0,0,0};
    }
}

void::LevelView::changeBits(int cupBitNum, int ballBitHexNum, bool isActive)
{
    if (level == 3)
    {
        // Call overloaded method
        updateBits(cupBitNum, ballBitHexNum, isActive);
    }
    else
    {
        updateBits(cupBitNum, isActive);
        updateBitLabel(cupBitNum, isActive);
    }
}

void::LevelView::clickedBits1()
{
    ball1 = !ball1;
    emit clickedBits(1, ball1);
    updateBitLabel(1, ball1);
}

void::LevelView::clickedBits2()
{
    ball2 = !ball2;
    emit clickedBits(2, ball2);
    updateBitLabel(2, ball2);
}

void::LevelView::clickedBits4()
{
    ball4 = !ball4;
    emit clickedBits(3, ball4);
    updateBitLabel(3, ball4);
}

void::LevelView::clickedBits8()
{
    ball8 = !ball8;
    emit clickedBits(4, ball8);
    updateBitLabel(4, ball8);
}

void::LevelView::clickedBits16()
{
    ball16 = !ball16;
    emit clickedBits(5, ball16);
    updateBitLabel(5, ball16);
}

void::LevelView::clickedBits32()
{
    ball32 = !ball32;
    emit clickedBits(6, ball32);
    updateBitLabel(6, ball32);
}

void::LevelView::clickedBits64()
{
    ball64 = !ball64;
    emit clickedBits(7, ball64);
    updateBitLabel(7, ball64);
}

void::LevelView::clickedBits128()
{
    ball128 = !ball128;
    emit clickedBits(8, ball128);
    updateBitLabel(8, ball128);
}

void::LevelView::updateBitLabel(int index, bool on)
{
    if(on)
        bitLabels[index]->setText("1");
    else
        bitLabels[index]->setText("0");
}

void::LevelView::h1clicked()
{
    hex1 = !hex1;
    char c = 'A';
    emit clickedBits(1, c, hex1);

    bool on;
    for (int i = 1; i < 5; i++)
    {
        if (bits[i] == 1)
            on = true;
        else
            on = false;

        updateBitLabel(i,on);
    }
}

void::LevelView::h2clicked()
{
    hex2 = !hex2;
    char c = '1';
    emit clickedBits(2, c, hex2);

    bool on;
    for (int i = 5; i < 9; i++)
    {
        if (bits[i] == 1)
            on = true;
        else
            on = false;

        updateBitLabel(i,on);
    }
}

void::LevelView::updateScore(int score)
{
    ui->score->display(score);
}

void::LevelView::stopRound()
{
    this->close();
    emit timedOut();
}
