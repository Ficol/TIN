#ifndef GAME_H
#define GAME_H

#include "common.h"

#include "Player.h"

#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <chrono>

class Game
{
public:
    Game();
    std::vector<char> getSettings() const;
    char update();
    void changeState(const char id, const std::vector<char> &commands);
    std::vector<char> getState() const;
    void addPlayer(const char id);
    void removePlayer(const char id);

private:
    std::vector<Player> players;
    std::vector<Bullet> bullets;
    std::chrono::time_point<std::chrono::steady_clock> last_update;

    void shoot(Player &player, const size_t direction);
    void handleCollision();
    void reset();
    bool isShot(const game::Position player_position, const game::Position bullet_position) const;
};

#endif // GAME_H