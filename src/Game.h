#ifndef GAME_H
#define GAME_H

#include "common.h"

#include "Player.h"

#include <utility>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <mutex>

class Game
{
public:
    using Position = std::pair<size_t, size_t>;
    Game(const game::settings settings_);
    std::vector<char> getSettings() const;
    std::vector<char> getPoints() const;
    char update();
    void changeState(char id,  const std::string commands);
    std::vector<char> getState() const;
    void addPlayer(const char id);
    void removePlayer(const char id);

private:
    std::vector<Player> players;
    const game::settings settings;

    char checkCollision();
    void reset();
};

#endif // GAME_H