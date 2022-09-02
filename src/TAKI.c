#include "header.h"


int main()
{
    srand(time(NULL)); // Generate a random seed for the random functions, using the computer's internal clock.

    GAME_DATA game_data; // The game settings, containing the number of players, current top card, and the game's direction.

    // Print welcome message.
    Print_Welcome_Screen();

    // Initialize the game's data.
    Init_Game_Data(&game_data);

    // Get the number of players in the game.
    Set_Nof_Players(&game_data.nof_players);

    // Allocate memory for the data of all the players in the game.
    Init_Allocate_Players(&game_data);

    // Allocate memory for the arrays of each player's cards. Every player will start with the number of start cards const (4).
    Init_Allocate_Players_Cards(&game_data, NOF_START_CARDS);

    // Get the name  of each player in the game.
    Set_Players_Names(game_data.players, game_data.nof_players);

    // Give every player in the game his start cards (NOF_START_CARDS = 4).
    Hand_Start_Cards(&game_data, game_data.players, game_data.nof_players);

    // Start playing the game.
    Play_Game(&game_data);

    // Sort the game's statistics.
    Sort_Stats_Array(&game_data);

    // Print the game's statistics.
    Print_Game_Stats(game_data);

    // Free the memory allocated by malloc function for the cards arrays for all the players.
    Free_Cards_Arrays(game_data.players, game_data.nof_players);

    // Free the memory allocated by malloc function for the players array.
    free(game_data.players);

    return 0;
}