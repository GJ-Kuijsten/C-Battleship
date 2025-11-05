#include "player.h"
#include "Board.h"
#include "constants.h"
#include <iostream>
#include <cstdlib>
#include <queue>
#include <set>

Player::Player(const std::string& name, bool isAI) : name(name), isAI(isAI) {}


void Player::place_ships() {
    // prompt to let the player know he can place his ships
    std::cout << name << " can now place their ships:\n";

    // looping through each ship to place it
    for (int i = 0; i < shipSizes.size(); ++i) {
        int row, col, length = shipSizes[i];
        bool horizontal;
        char orientation;

        while (true) {
            if (isAI) {
                // placing the ships randomly
                row = rand() % BOARD_SIZE;
                col = rand() % BOARD_SIZE;
                horizontal = rand() % 2;
            } else {
                // player can place his ships as input: e.g. 1ah
                std::cout << "Enter a row (1-10), a column (A-J), and an orientation (H for horizontal, V for vertical) for ship "
                          << i + 1 << " (The length of the ship: " << length << "): ";
                char colChar;
                std::cin >> row >> colChar >> orientation;

                // converting to uppercase to prevent invalid inputs
                colChar = toupper(colChar);
                orientation = toupper(orientation);

                // converting the row and column and orientation with 0-indexing.
                row -= 1;
                col = colChar - 'A';
                horizontal = (orientation == 'H');

                // checking if input is correct
                if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE ||
                    (orientation != 'H' && orientation != 'V')) {
                    std::cout << "Invalid input. Please ensure row (1-10), column (A-J), and orientation (H or V) are correct.\n";
                    continue; // prompting the player again
                }
            }

            // checking if the ship can be placed
            if (board.canplace_ship(row, col, length, horizontal)) {
                // placing the ship
                board.placeShip(row, col, length, horizontal);
                break;
            } else if (!isAI) {
                std::cout << "Invalid placement: Ship overlaps or is out of bounds. Please try again.\n";
            }
        }
    }

    // showing the grid of the player after placement of the ships
    if (!isAI) {
        std::cout << name << "'s grid after ship placement:\n";
        board.display(true);
    } else {
        std::cout << "A. Smith has placed all ships.\n";
    }
}



std::string Player::shoot_coordinates(Player& opponent) {
    static std::vector<std::pair<int, int>> hillclimbing_path; // tracks AI's target cells after a hit
    static std::set<std::pair<int, int>> visited_cells;        // tracks all cells already shot by the AI

    int row, col;

    // evaluating how good a cell is to shoot at (calculates score based on how many of its neighboring cells are unvisited)
    auto heuristic_func = [&](int r, int c) -> int {
        int score = 0;
        // increasing the score for cells near unvisited neighbours, so there is more chance to hit a ship
        if (r > 0 && visited_cells.find({r - 1, c}) == visited_cells.end()) score++;
        if (r < BOARD_SIZE - 1 && visited_cells.find({r + 1, c}) == visited_cells.end()) score++;
        if (c > 0 && visited_cells.find({r, c - 1}) == visited_cells.end()) score++;
        if (c < BOARD_SIZE - 1 && visited_cells.find({r, c + 1}) == visited_cells.end()) score++;
        return score;
    };

    // evaluate neighbors to find the best cell to shoot at
    auto findBest_Neighbor = [&](int r, int c) -> std::pair<int, int> {
        std::vector<std::pair<int, int>> neighbors;
        if (r > 0) neighbors.emplace_back(r - 1, c); // if r > 0, the cell above is valid and added to the neighbors vector
        if (r < BOARD_SIZE - 1) neighbors.emplace_back(r + 1, c); // below
        if (c > 0) neighbors.emplace_back(r, c - 1); // if c > 0, the cell to the left is valid and is added to the neighbors vector
        if (c < BOARD_SIZE - 1) neighbors.emplace_back(r, c + 1); // to the right

        // choosing the best neighbor based on the heuristic
        std::pair<int, int> best = {-1, -1};
        int bestScore = -1;
        for (auto& neighbor : neighbors) {
            if (visited_cells.find(neighbor) == visited_cells.end()) { // only considering cells that are unvisited
                int score = heuristic_func(neighbor.first, neighbor.second); // assigning score with heuristic function
                // taking the best score
                if (score > bestScore) {
                    bestScore = score;
                    best = neighbor;
                }
            }
        }
        return best;
    };

    if (isAI) {
        if (hillclimbing_path.empty()) {
            // randomly selecting an unvisited cell and continue till a ship is hit
            do {
                row = rand() % BOARD_SIZE;
                col = rand() % BOARD_SIZE;
            } while (visited_cells.find({row, col}) != visited_cells.end());
        } else {
            // when a ship is hit, the AI starts picking the next cell from the hill climbing path and removes it
            // making use of the queue library
            auto nextMove = hillclimbing_path.back();
            hillclimbing_path.pop_back(); // removing using pop_back()
            row = nextMove.first;
            col = nextMove.second;
        }

        // shooting at the selected cell and mark it as visited
        visited_cells.insert({row, col});
        std::string result = opponent.board.shoot(row, col);



        if (result == "Hit!") {
            // adding neighbors to the hill climbing path using push_back()
            auto best_neighbor = findBest_Neighbor(row, col);
            if (best_neighbor.first != -1 && best_neighbor.second != -1) {
                hillclimbing_path.push_back(best_neighbor);
            }
        }

        return result;
    } else {
        // human player shooting
        while (true) {
            std::cout << name << ", enter a row (1-10) and a column (A-J) to shoot: ";
            char colChar;
            std::cin >> row >> colChar;

            // converting the input of the player to uppercase and 0-indexing
            colChar = toupper(colChar);
            row -= 1;
            col = colChar - 'A';

            // check if input is valid, else print error message
            if (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE) {
                break;
            } else {
                std::cout << "Your input is invalid. Please try again.\n";
                std::cin.clear(); // clears the error
                std::cin.ignore(10000, '\n'); // ignoring up to 10000 characters or until a newline
            }
        }

        return opponent.board.shoot(row, col);
    }
}



// check if all ships on the player's board have been destroyed
bool Player::has_Lost() const {
    return board.allships_destroyed();
}
// returns the name of the player
const std::string& Player::get_Name() const {
    return name;
}

// displaying the board of the player
void Player::display_Grid(bool hideShips) const {
    board.display(hideShips);
}
