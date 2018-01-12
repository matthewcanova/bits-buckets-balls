#ifndef GAMEOVERDIALOG_H
#define GAMEOVERDIALOG_H


#include <QMainWindow>

namespace Ui {
class GameOverDialog;
}

class GameOverDialog : public QMainWindow
{
    Q_OBJECT

public:
    explicit GameOverDialog(QWidget *parent = 0);
    ~GameOverDialog();
    void updateScore(int);
    void updateForTimeout();

signals:
    void accepted();
    void rejected();

private:
    Ui::GameOverDialog *ui;

private slots:
    void yesClicked();
    void noClicked();

};


#endif // GAMEOVERDIALOG_H
