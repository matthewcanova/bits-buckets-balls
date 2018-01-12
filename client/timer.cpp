#include "timer.h"
#include <iostream>

Timer::Timer(QObject *parent) : QObject(parent)
{
    timerPtr = new QTimer(parent);
    connect(timerPtr, SIGNAL(timeout()), this, SLOT(updateTimer()));
}

Timer::~Timer()
{
    delete timerPtr;
    timerPtr = nullptr;
}

void Timer::startTime()
{
    time.start();
    timerPtr->start(100);
}

void Timer::stopTime()
{
    timerPtr->stop();
}

void Timer::updateTimer()
{
    clock = time.elapsed();
    emit updateTime(clock);

    //times out if greater than 5 minutes
    if (clock >= 500000)
    {
        emit stopRound();
        stopTime();
    }
}

int Timer::returnCurrentTime()
{
    clock = time.elapsed();
    return clock;
}
