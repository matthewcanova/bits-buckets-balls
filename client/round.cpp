#include "round.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <random>
#include <bitset>

Round::Round(int level, int round)
{
    currLevel = level;
    currRound = round;
    std::srand(time(NULL));
    num = getNum();

}
bool Round::checkAnswer(std::string answer)
{
    if(currLevel == 1) return Level1(answer);
    else if (currLevel == 2) return Level2(answer);
    else if (currLevel == 3) return Level3(answer);
}

int Round::getLevel()
{
    return currLevel;
}

int Round::getRound()
{
    return currRound;
}

void Round::setLevel(int level)
{
    currLevel = level;
    num = getNum();
}

void Round::setRound(int round)
{
    currRound = round;
    num = getNum();
}

int Round::getNum()
{
    if(currLevel == 1)
    {
        if(currRound <= 5)
        {
            std::random_device generator;
            std::uniform_int_distribution<int> distribution(1,15);
            return distribution(generator);
        }
        else if (currRound <= 11)
        {
            std::random_device generator;
            std::uniform_int_distribution<int> distribution(16,63);
            return distribution(generator);
        }
        else if (currRound <= 15)
        {
            std::random_device generator;
            std::uniform_int_distribution<int> distribution(64,255);
            return distribution(generator);
        }
    }

    if(currLevel == 2)
    {
        if(currRound <= 5)
        {
            std::random_device generator;
            std::uniform_int_distribution<int> distribution(-8,7);
            return distribution(generator);
        }
        else if (currRound <= 11)
        {
            std::random_device generator;
            std::uniform_int_distribution<int> distribution(-32,31);
            int test = distribution(generator);
            while(test >= -8 && test <= 7 )
            {
                test = distribution(generator);
            }
            return test;
        }
        else if (currRound <= 15)
        {
            std::random_device generator;
            std::uniform_int_distribution<int> distribution(-128,127);
            int test = distribution(generator);
            while(test >= -32 && test <= 31 )
            {
                test = distribution(generator);
            }
            return test;
        }
    }
    if(currLevel == 3)
    {
        if(currRound <= 5)
        {
            std::random_device generator;
            std::uniform_int_distribution<int> distribution(1,15);
            return distribution(generator);
        }
        else if (currRound <= 10)
        {
            std::random_device generator;
            std::uniform_int_distribution<int> distribution(16,127);
            return distribution(generator);
        }
        else if (currRound <= 15)
        {
            std::random_device generator;
            std::uniform_int_distribution<int> distribution(128,255);
            return distribution(generator);
        }
    }
}

bool Round::Level1(std::string answer)
{
    int ans = std::stoi(answer,nullptr,2);
    return (ans == num);
}

bool Round::Level2(std::string answer)
{
    //twos complement. Flip bits add one.
    return true;
}

bool Round::Level3(std::string answer)
{
    int ans = std::stoi(answer,nullptr,16);
    return (ans == num);
}

/*Returns the calculated score.  Score is based on the time
 * taken to complete the round.  Maximum score is 1000.  For each second
 * used, 10 points is deducted from the maximum score (up to a minute, for a
 * total deduction of 600 points).  After one minute, 5 points is deducted for
 * each additional second.  After two minutes, the base score is 100.*/
int Round::calculateScore (int time)
{
    int seconds, score;
    //max score
    score = 1000;
    seconds = time / 1000;
    if (seconds >= 120)
    {
        score = 100;
    }
    else if (seconds >= 60)
    {
        score = 400 - 5 * (seconds % 60);
    }
    else
        score = 1000 - 10 * (seconds % 60);

    return score;
}
