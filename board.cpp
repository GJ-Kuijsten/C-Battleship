#include "board.h"
#include "constants.h"
#include <iostream>
#include <stdexcept>

Board::Board() : grid(BOARD_SIZE, std::vector<char>(BOARD_SIZE, EMPTY_CELL)), ships_remaining(0) {}


void Board::placeShip(int row, int col, int length, bool horizontal) {
    if (!canplace_ship(row, col, length, horizontal)) {
        throw std::invalid_argument("Invalid ship placement.");
    }

    if (horizontal) {
        for (int i = 0; i < length; ++i) {
            grid[row][col + i] = 'S'; // marking cells with 'S' for ship
        }
    } else {
        for (int i = 0; i < length; ++i) {
            grid[row + i][col] = 'S'; // marking cells with 'S' for ship
        }
    }
    ships_remaining += length; // updating the total ship cells
}





std::string Board::shoot(int row, int col) {
    if (grid[row][col] == SHIP_CELL) {
        grid[row][col] = HIT_CELL;
        ships_remaining--;
        return "Hit!";
    } else if (grid[row][col] == EMPTY_CELL) {
        grid[row][col] = MISS_CELL;
        return "Miss!";
    }
    return "Already Shot!";
}

bool Board::allships_destroyed() const {
    return ships_remaining == 0;
}

void Board::display(bool showShips) const {
    std::cout << "   A B C D E F G H I J\n"; // Columns A-I of the board
    for (int row = 0; row < BOARD_SIZE; ++row) {
        std::cout << (row + 1); // row numbers 1-10 and 1-indexed
        if (row + 1 < 10) std::cout << " "; // Aligning the rows, because 10 is two-digits

        for (int col = 0; col < BOARD_SIZE; ++col) {
            if (grid[row][col] == 'S' && showShips) {
                std::cout << " S"; // showing ships when allowed (HideShips parameter must be true)
            } else if (grid[row][col] == 'S') {
                std::cout << " ~"; // hiding ships as water
            } else {
                std::cout << " " << grid[row][col]; // showing hits as ('X') or misses as ('O')
            }
        }
        std::cout << "\n";
    }
}

// checking if a ship can be placed on the given position
bool Board::canplace_ship(int row, int col, int length, bool horizontal) const {
    if (horizontal) {
        if (col + length > BOARD_SIZE) return false;
        for (int i = 0; i < length; ++i) {
            if (grid[row][col + i] != EMPTY_CELL) return false;
        }
    } else {
        if (row + length > BOARD_SIZE) return false;
        for (int i = 0; i < length; ++i) {
            if (grid[row + i][col] != EMPTY_CELL) return false;
        }
    }
    return true;
}
