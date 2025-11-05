#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <string>

class Board {
private:
    std::vector<std::vector<char>> grid;
    int ships_remaining;

public:
    Board();
    void placeShip(int row, int col, int length, bool horizontal);
    std::string shoot(int row, int col);
    bool allships_destroyed() const;
    void display(bool hideShips = false) const;
    bool canplace_ship(int row, int col, int length, bool horizontal) const;
};

#endif
