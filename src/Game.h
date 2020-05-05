#ifndef GAME_H
#define GAME_H

class Game
{
public:
    Game(int sum_) : sum(sum_)
    {
    }
    void changeState(int number)
    {
        sum += number;
    }
    int getState()
    {
        return sum;
    }

private:
    int sum;
};

#endif // GAME_H