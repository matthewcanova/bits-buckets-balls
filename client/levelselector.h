#ifndef LEVELSELECTOR_H
#define LEVELSELECTOR_H

#include <QDialog>
#include <QPixmap>
#include <QLabel>

namespace Ui {
class levelSelector;
}

class levelSelector : public QDialog
{
    Q_OBJECT

public:
    explicit levelSelector(QWidget *parent = 0);
    int getLevel();
    ~levelSelector();

private slots:
    void level1Selected(bool clicked);
    void level2Selected(bool clicked);
    void level3Selected(bool clicked);
    void clickedLabel(QString round);
    void updateStars(int*);

signals:
    void startRound(int, int);
    void getScores(int);

private:
    Ui::levelSelector *ui;
    bool level1;
    bool level2;
    bool level3;

    void initializeStarArrays();
    QLabel *star1Labels[16], *star2Labels[16], *star3Labels[16], *star4Labels[16];
};

#endif // LEVELSELECTOR_H
