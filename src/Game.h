#ifndef GAME_H
#define GAME_H

class Game
{
public:
    Game(int sum_);
    void changeState(int number);
    int getState();

private:
    int sum;
};

#endif // GAME_H