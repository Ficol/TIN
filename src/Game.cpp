#include "Game.h"

Game::Game(int sum_) : sum(sum_)
{
}
void Game::changeState(int number)
{
    sum += number;
}
int Game::getState()
{
    return sum;
}