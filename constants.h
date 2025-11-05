#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <vector>


constexpr int BOARD_SIZE = 10; // grid size of 10x10
constexpr int MAX_SHIPS = 5; // each player can only have 5 ships
constexpr int MAX_BOMBS = 100; // all bombs that can be used
constexpr char EMPTY_CELL = '~'; // water
constexpr char SHIP_CELL = 'S'; // ship
constexpr char HIT_CELL = 'X'; // bomb hit
constexpr char MISS_CELL = 'O'; //  bomb miss

// length of the ships
inline std::vector<int> shipSizes = {5, 4, 3, 3, 2};

#endif
