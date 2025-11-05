#ifndef PLAYER_H
#define PLAYER_H

#include "Board.h"
#include <string>

class Player {
private:
    std::string name;
    Board board;
    bool isAI;

public:
    Player(const std::string& name, bool isAI = false);
    void place_ships();
    std::string shoot_coordinates(Player& opponent);
    bool has_Lost() const;
    const std::string& get_Name() const;
    void display_Grid(bool hideShips = false) const;
};

#endif
