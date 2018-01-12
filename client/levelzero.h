#ifndef LEVELZERO_H
#define LEVELZERO_H

#include <QMainWindow>
#include <QTimer>
#include <SFML/Graphics.hpp>

namespace Ui {
class LevelZero;
}

class LevelZero : public QMainWindow
{
    Q_OBJECT

public:

    explicit LevelZero(QWidget *parent = 0);
    LevelZero(QWidget *parent, int level);
    ~LevelZero();
    void updateTimeAndScore(int time, int score);

private:
    Ui::LevelZero *ui;



signals:

public slots:
};

#endif // LEVELZERO_H
