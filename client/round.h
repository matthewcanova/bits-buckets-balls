#ifndef ROUND_H
#define ROUND_H
#include <string>

class Round
{
public:
    Round(int level, int round);
    bool checkAnswer(std::string answer);
    int getLevel();
    int getRound();
    void setLevel(int level);
    void setRound(int round);
    int getNum();
    int calculateScore(int time);

private:
    int currLevel;
    int currRound;
    int num;
    bool Level1(std::string answer);
    bool Level2(std::string answer);
    bool Level3(std::string answer);

};

#endif // ROUND_H
