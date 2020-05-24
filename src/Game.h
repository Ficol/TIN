#ifndef GAME_H
#define GAME_H

#include "common.h"

#include "Player.h"

#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <chrono>
#include <mutex>

class Game
{
public:
    Game(const game::settings settings_);
    std::vector<char> getSettings() const;
    std::vector<char> getPoints() const;
    void update(std::vector<char> &scorers);
    void changeState(char id, const std::vector<char> &commands);
    std::vector<char> getState() const;
    void addPlayer(const char id);
    void removePlayer(const char id);

private:
    std::vector<Player> players;
    std::vector<Bullet> bullets;
    const game::settings settings;
    std::chrono::time_point<std::chrono::steady_clock> last_update;

    void shoot(char id, game::Position position, size_t direction);
    void handleCollision(std::vector<char> &scorers);
    void reset();
    bool isShot(game::Position player_position, game::Position bullet_position);
};

#endif // GAME_H