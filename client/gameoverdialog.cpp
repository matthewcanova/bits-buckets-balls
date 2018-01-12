#include "gameoverdialog.h"
#include "ui_gameover.h"
#include <QPixmap>


GameOverDialog::GameOverDialog(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GameOverDialog)
{
    ui->setupUi(this);

    connect(ui->yesButton, SIGNAL(clicked()), this, SLOT (yesClicked()));
    connect(ui->noButton, SIGNAL(clicked()), this, SLOT (noClicked()));

}

GameOverDialog::~GameOverDialog()
{
    delete ui;
}

/*Emits signal when yes button is clicked*/
void GameOverDialog::yesClicked()
{
    emit accepted();
}

/*Emits signal when no button is clicked*/
void GameOverDialog::noClicked()
{
    emit rejected();
}

/*Updates the score shown on the display and the appropriate number of stars
 * corresponding to the score.  Also, displays different messages depending upon the
 * score received.*/
void GameOverDialog::updateScore(int score)
{
    QString text = QString::number(score);
    ui->score->setText(text);

    ui->star1->clear();
    ui->star2->clear();
    ui->star3->clear();
    ui->star4->clear();
    QPixmap star ("../resources/star2.png");

    if (score > 0)
    {
        ui->infoLabel->setText("Good effort");
        ui->star1->setPixmap(star);
    }

    if (score > 300)
    {
        ui->infoLabel->setText("Keep up the good work!");
        ui->star2->setPixmap(star);
    }

    if  (score > 600)
    {
        ui->infoLabel->setText("Nice Work!");
        ui->star3->setPixmap(star);
    }

    if(score > 900)
    {
        ui->infoLabel->setText("You are a bit wizard!");
        ui->star4->setPixmap(star);
    }
}

/*Updates the display if the game timed out*/
void GameOverDialog::updateForTimeout()
{
    ui->score->hide();
    ui->scoreLabel->hide();
    ui->infoLabel->setText("Your game timed out.");
}
