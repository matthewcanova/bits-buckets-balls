#ifndef TIMER_H
#define TIMER_H

#include <QObject>
#include <QTime>
#include <QTimer>

class Timer : public QObject
{
    Q_OBJECT

public:
    explicit Timer(QObject *parent = 0);
    ~Timer();

    void startTime();
    void stopTime();
    int returnCurrentTime();

signals:
    void updateTime(int);
    void stopRound();

public slots:
    void updateTimer();

private:

    QTime time;
    QTimer* timerPtr;
    int totalTime;
    int clock;

};

#endif // TIMER_H
