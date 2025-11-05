#include "board.h"
#include "player.h"
#include <iostream>
#include <ctime>


// function to play the game
void game(Player& player1, Player& player2) {
    Player* curr = &player1;
    Player* opp = &player2;
    int moves = 0; // counting the total valid shots

    while (true) {
        // curr player's turn
        std::cout << curr->get_Name() << "'s turn:\n";
        opp->display_Grid(false); // opps board with ships hidden

        // the curr player takes a shot
        std::string result = curr->shoot_coordinates(*opp);
        std::cout << result << "\n";

        // incrementing move counter if it is a valid move
        if (result == "Hit!" || result == "Miss!") {
            moves++;
        }
        // checking if the ships of the opp are all destroyed
        if (opp->has_Lost()) {
            std::cout << curr->get_Name() << " wins in " << moves/2 << " moves!\n";
            break;
        }

        // if shot is missed, swap turns
        if (result == "Miss!") {
            std::swap(curr, opp);
        }
    }
}




int main() {
    srand(time(nullptr)); // random seed, so the sequence of random numbers generated are different each time running the program

    Player player1("Player 1");
    Player player2("A. Smith", true);

    // showing the grid before placing ships
    std::cout << "Empty grid's:\n";
    std::cout << "Player 1's grid:\n";
    player1.display_Grid(false); // showing empty board for the player
    std::cout << "A. Smith's grid:\n";
    player2.display_Grid(false); // showing empty board for A. Smith

    // placing the ships for both players
    player1.place_ships();
    player2.place_ships();

    // starting the game
    game(player1, player2);

    return 0;
}

