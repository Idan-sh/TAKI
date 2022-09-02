#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define MAX_NAME_LEN 21 // The maximum length of the first name of each player. the maximum length is 20, and the last spot is for the '\0' closing character.
#define MAX_CARD_NAME_LEN 7 // The maximum length of the special card's type string. The longest string is NORMAL which contains 7 characters- the last is the closing character '\0')

// The dimensions of each card:
#define CARD_LENGTH 6
#define CARD_WIDTH 9
#define CARD_BORDER "*" // The character of the border of the cards

// The starting count of how many cards each player has.
#define NOF_START_CARDS 4

#define EMPTY -1 // Empty card number will be represented by -1, so the card number value in the special cards will be initialized to -1.

// Define the characters for the cards' colors:  (green/red/yellow/blue).
#define GREEN 'G'
#define RED 'R'
#define YELLOW 'Y'
#define BLUE 'B'
#define NO_COLOR ' '
#define ERROR 'E'
#define NUM_OF_COLORS 4 // The number of card colors in the game.

// Define the types of cards: "+" / "STOP" / "<->" / "COLOR" / "TAKI" / "NORMAL".
#define PLUS "+"
#define STOP "STOP"
#define DIRECTION "<->"
#define COLOR "COLOR"
#define TAKI "TAKI"
#define NORMAL "NORMAL"
#define NOF_CARD_TYPES 6 // The number of possible card types.

#define GAME_STATS_MAX_SIZE 14 // The possible stats are for 9 number cards and 5 special cards, 14 in total.
#define STAT_EMPTY "EMPTY" // Indicates that the stat is empty.
#define COL_LEN 7 // The length of the stats collum.




// Card data containing its characteristics.
typedef struct Card
{
    char type[MAX_CARD_NAME_LEN]; // The possible type names are: "+" / "STOP" / "<->" / "COLOR" / "TAKI" / "NORMAL". ("NORMAL" is for the normal cards that have a number in them, other types are the special cards)
    int num; // Containing a number from 1-9.
    char color; // Each color is represented by the first character of its name: 'G' / 'R' / 'Y' / 'B'. (green/red/yellow/blue)
} CARD;

// Player data containing his name and an array of all the cards that in his possession.
typedef struct Player
{
    char name[MAX_NAME_LEN]; // The name of the player. The maximum length of the name is MAX_NAME_LEN - 21. (the actual length of the max name is 20, and the last character is the closing character '\0\)
    CARD* cards; // An array containing all the cards that belong to the player, the array is dynamic.
    int nof_cards; // The number of cards that the player has. This is both the physical and logical size of the cards array.
    int cards_phys_size; // The physical size of the cards array.
} PLAYER;

// Statistic data: The card number or type and the frequency of how many times that card was drawn.
typedef struct Stat_Data
{
    char card_type[MAX_CARD_NAME_LEN]; // The card number or type.
    int card_num;
    int card_freq; // The number of times the card was drawn.
} STAT_DATA;

// Game data containing the players and the game's logic.
typedef struct Game_Data
{
    PLAYER* players; // Pointer to array of all the players in the game.
    int nof_players; // The number of players in the game.
    int player_index; // The index of the player whose turn it is.
    CARD top_card; // The card on the top of the deck.
    bool is_direction_right; // True if the direction of the play is to the right, false if it is to the left.
    bool is_game_won; // If the game has finished, one of the players dropped all his cards.
    STAT_DATA stats[GAME_STATS_MAX_SIZE]; // Array of stats of all the cards drawn.
    int nof_stats; // The number of stats currently in the stats array.
} GAME_DATA;


// ---------------------- Print Functions -----------------------

void Print_Welcome_Screen();

void Print_Card(CARD card);

void Print_Normal_Card(CARD card);

void Print_Special_Card(CARD card);

void Print_Player_Cards(PLAYER player);

void Print_Current_Deck(CARD top_card, PLAYER player);

void Print_Game_Stats(GAME_DATA game_data);

void Print_Special_Card_Stat(STAT_DATA stat);

// ------------------ Player Input Functions --------------------

void Set_Nof_Players(int* nof_players);

void Set_Players_Names(PLAYER players_data[], int size);

void Hand_Start_Cards(GAME_DATA* game_data_p, PLAYER players[], int nof_players);

// -------------------- Randomize Functions ---------------------

void Take_Random_Card(CARD* result_card_p);

void Get_Random_Normal_Card(CARD* result_card_p);

char Get_Random_Color();

char Get_Color_Char(int color_num);

// ---------- Allocation and Initialization Functions ----------

void Init_Game_Data(GAME_DATA* game_data_p);

void Init_Allocate_Players(GAME_DATA* game_data_p);

void Init_Allocate_Players_Cards(GAME_DATA* game_data_p, int size);

void Reallocate_Cards_Array(PLAYER* player_p, int size);

void Free_Cards_Arrays(PLAYER players[], int nof_players);

// -------------------- Play Cards Functions --------------------

bool Check_Normal_Card(CARD normal_card, CARD top_card);

bool Check_Special_Card(CARD special_card, CARD top_card);

bool Try_Play_Card(GAME_DATA* game_data_p, PLAYER* player_p, int card_i);

bool Play_Normal_Card(PLAYER* player_p, int card_i, CARD* top_card_p);

void Play_Color_Card(PLAYER* player, int card_i);

void Play_Stop_Card(GAME_DATA* game_data_p, PLAYER* player_p);

void Play_Direction_Card(GAME_DATA* game_data_p);

void Play_Taki_Card(GAME_DATA* game_data_p, PLAYER* player_p, int taki_card_i);

void Play_Plus_Card(GAME_DATA* game_data_p, PLAYER* player_p, int card_i);

void Remove_Card_From_Array(PLAYER* player_p, int card_i, CARD* top_card_p);

void Draw_New_Card(GAME_DATA* game_data_p, PLAYER* player_p);

// -------------------- Gameplay Functions --------------------

void Play_Game(GAME_DATA* game_data_p);

bool Check_Winner(PLAYER player);

int Find_Str_Mid_Index(char str[]);

// ------------------- Statistics Functions --------------------

void Add_Game_Stat(GAME_DATA* game_data_p, CARD card);

void Check_Stat_Normal_Card(GAME_DATA* game_data_p, CARD card);

void Check_Stat_Special_Card(GAME_DATA* game_data_p, CARD card);

void Sort_Stats_Array(GAME_DATA* game_data_p);

void Swap_Stats(STAT_DATA stats[], int first_i, int second_i);


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


/*
 * Prints the welcome message of the game.
 */
void Print_Welcome_Screen()
{
    // Print message.
    printf("************  Welcome to TAKI game !!! ***********\n");
}


/*
 * Get the number of players that in the game.
 * Receives the pointer to the nof_players variable that will be set in the main function.
 * Sets the number if players value into that variable.
 */
void Set_Nof_Players(int* nof_players)
{
    // Print request for the number of players in the game.
    printf("Please enter the number of players:\n");
    // Get the input and enter it into the received location of nof_players variable.
    scanf("%d", nof_players);
}


/*
 * Get the names of all the players in the game.
 * Receives players array, and the number of players in the game.
 * The maximum length of each name is MAX_NAME_LEN (21), where the last character is for the closing character '\0'.
 */
void Set_Players_Names(PLAYER players[], int nof_players)
{
    // For each player in players array.
    for(int i = 0; i < nof_players; i++)
    {
        // Request name input for the player in index i. (we start counting players from 1, so the player number is (i+1))
        printf("Please enter the first name of player #%d:\n", i+1);
        // Get the name into this player's name.
        scanf("%s", players[i].name);
    }
}


/*
 * Initialize the game's data:
 * The starting player index, the game won status, the direction of the turns
 * and the card starting the deck of cards.
 */
void Init_Game_Data(GAME_DATA* game_data_p)
{
    game_data_p->player_index = 0; // Initialize the index of the current player playing.
    game_data_p->is_game_won = false; // Initialize the game won to be false.
    game_data_p->is_direction_right = true; // Initialize the direction of the game to the right.

    // Get a random first card in the game, sets that card on the top of the card deck.
    Get_Random_Normal_Card(&game_data_p->top_card);

    game_data_p->nof_stats = 0; // Initialize the game stats to the logic size of 0.

    // Initialize every stat is the stats array.
    for (int i = 0; i < GAME_STATS_MAX_SIZE; i++)
    {
        strcpy(game_data_p->stats[i].card_type, STAT_EMPTY); // Initialize the stats' names to be "EMPTY" (when a stat will be added this value will be overwritten).
        game_data_p->stats[i].card_num = EMPTY; // Initialize the number to be empty.
        game_data_p->stats[i].card_freq = 0; // Initialize the card frequency;
    }
}


/*
 * Initialize the allocation of the players' cards.
 * Receives a pointer to the game's data, which contains all the data for the players.
 * Receives the wanted size for the array of players' cards.
 * Allocates memory for the cards array for all the starting cards.
 * If the allocation failed, prints error message and ends the program.
 */
void Init_Allocate_Players_Cards(GAME_DATA* game_data_p, int size)
{
    // For each player in the game.
    for (int player_i = 0; player_i < game_data_p->nof_players; player_i++)
    {
        // Try to allocate enough space for the starting cards of that player.
        game_data_p->players[player_i].cards = (CARD *) malloc(sizeof(CARD) * size);

        game_data_p->players[player_i].cards_phys_size = size; // Initialize each player's cards array physical size.

        // Check if the allocation failed.
        if (game_data_p->players[player_i].cards == NULL)
        {
            printf("Memory allocation failed!!!\n"); // Print error message.
            exit(1); // Stop the program.
        }
    }
}


/*
 * Initialize the allocation of the players' data.
 * Receives a pointer to the game's data, where the players array is stored.
 * Allocates memory for the players' data array.
 * If the allocation failed, prints error message and ends the program.
 */
void Init_Allocate_Players(GAME_DATA* game_data_p)
{
    // Try to allocate enough space for all the players.
    game_data_p->players = (PLAYER *) malloc(sizeof(PLAYER) * game_data_p->nof_players);

    // Check if the allocation failed.
    if (game_data_p->players == NULL)
    {
        // Print error message.
        printf("Memory allocation failed!!!\n");
        exit(1); // Stop the program.
    }
}


/*
 * Find the middle index of a string.
 * Receives a string to find its middle index.
 * If the string's length is an even number, the middle index will be to the left of the exact middle location.
 * Else, the string length is an odd number, so the middle index is the exact middle location.
 * Returns the result index.
 * (This function picks the LEFT index from the exact middle in even length strings)
 */
int Find_Str_Mid_Index(char str[])
{
    int i = 0; // The index of the current character in the string.
    int len = 0; // The count of the length of the string.

    // Count the length of the string.
    while (str[i] != '\0')
    {
        len++; // Add 1 to the length.
        i++; // Move to the next index in the string.
    }

    // Check if the length of the string is an even number.
    if (len % 2 == 0)
        return len / 2 - 1; // It's an even number, the middle index will be the number to the left of the exact middle.
    else
        return len / 2; // It's an odd number, so the middle index is that exact number.
}


/*
 * Find the middle index of a string.
 * Receives a string to find its middle index.
 * If the string's length is an even number, the middle index will be to the right of the exact middle location.
 * Else, the string length is an odd number, so the middle index is the exact middle location.
 * Returns the result index.
 * (This function picks the RIGHT index from the exact middle in even length strings)
 */
int Find_Str_Mid_Index2(char str[])
{
    int i = 0; // The index of the current character in the string.
    int len = 0; // The count of the length of the string.

    // Count the length of the string.
    while (str[i] != '\0')
    {
        len++; // Add 1 to the length.
        i++; // Move to the next index in the string.
    }

    return len / 2; // Return the middle index of the length of the string.
}


/*
 * Prints a normal card- containing a number from 1-9 and a color (green/red/yellow/blue).
 * Each color is represented by the first character of its name (G/R/Y/B).
 * Receives a card containing a card number and a color in the correct format.
 * Prints the card.
 */
void Print_Normal_Card(CARD card)
{
    int num = card.num; // Get the card's number.
    char color = card.color; // Get the card's color.

    // For each row of the card.
    for (int row = 0; row < CARD_LENGTH; row++)
    {
        // For each collum of that row.
        for (int col = 0; col < CARD_WIDTH; col++)
        {
            // If reached the top or bottom sections of the card, prints the border with '*' and continue to the next collum.
            if (row == 0 || row == CARD_LENGTH - 1) { printf(CARD_BORDER); continue; }

            // If reached the left or right borders of the card, prints the border with '*' and continue to the next collum.
            if (col == 0 || col == CARD_WIDTH - 1) { printf("*"); continue; }

            // Check if the current collum is in the middle of the card.
            if (col == CARD_WIDTH / 2)
            {
                // The card length is an even number so there are 2 rows in the middle- first row for the number, second row for the color.
                // Check if in the right row for the number of the card.
                if (row == CARD_LENGTH / 2 - 1)
                    printf("%d", num); // Print the card number
                else // Not in the first middle row.
                {
                    // Check if in the right row for the color of the card.
                    if (row == CARD_LENGTH / 2)
                        printf("%c", color); // Print the color's character.
                    else // Not in the second middle row.
                        printf(" "); // Print an empty space.
                }
            }
            else // Not in the middle collum.
                printf(" "); // Print an empty space.
        }
    printf("\n"); // Go down a line for the next row to start.
    }
}


/*
 * Prints a special card- containing the type: "+" / "STOP" / "<->" / "COLOR" / "TAKI" , and a color: green / red / yellow / blue.
 * Each color is represented by the first character of its name (G/R/Y/B).
 * Receives a card containing its type and color.
 * Prints the card.
 */
void Print_Special_Card(CARD card)
{
    int card_mid = CARD_WIDTH / 2, card_type_mid = Find_Str_Mid_Index(card.type); // Get the card's width middle and the card's type string middle.
    int print_index = card_mid - card_type_mid; // The collum index where the card's type will be printed. Set the start index depending on the card's width and type string.
    int type_index = 0; // The index of the type's character that needs to be printed.

    // For each row of the card.
    for (int row = 0; row < CARD_LENGTH; row++)
    {
        // For each collum of that row.
        for (int col = 0; col < CARD_WIDTH; col++)
        {
            // If reached the top or bottom sections of the card, prints the border with the card border character and continue to the next collum.
            if (row == 0 || row == CARD_LENGTH - 1) { printf(CARD_BORDER); continue; }

            // If reached the left or right borders of the card, prints the border with the card border character and continue to the next collum.
            if (col == 0 || col == CARD_WIDTH - 1) { printf(CARD_BORDER); continue; }

            // Check if in the right row for the type name.
            if (row == CARD_LENGTH / 2 - 1)
            {
                // Check if the current collum is in the location to print the type name, and if there are still characters in the type's name that weren't printed.
                if (col == print_index && card.type[type_index] != '\0')
                {
                    // Print the character of type name in the current index.
                    printf("%c", card.type[type_index]);
                    type_index++; // Move to the next index in the type name.
                    print_index++; // Move to the index of the next collum where the next type name character will be printed.
                }
                else // The collum is not in the correct place or the type name was printed completely.
                    printf(" "); // Print an empty space.

                continue; // Continue to the next collum.
            }

            // Check if the current collum is in the middle of the card where the color character will be printed.
            if (col == CARD_WIDTH / 2 && row == CARD_LENGTH / 2)
            {
                printf("%c", card.color); // Print the color's character.
            }
            else // Not in the correct location for the color.
                printf(" "); // Print an empty space.
        }
        printf("\n"); // Go down a line for the next row to start.
    }
}


/*
 * Prints the received card's info.
 * Checks whether the card is a normal card or special card,
 * Calls for the right print function that will print the card.
 */
void Print_Card(CARD card)
{
    // Check if the card is a NORMAL card.
    if (!strcmp(card.type, NORMAL))
        Print_Normal_Card(card); // The card type is NORMAL, then prints with the normal cards functions.
    else
        Print_Special_Card(card); // The card type isn't NORMAL (the type is one of the special cards), then prints with the special cards functions.
}


/*
 * Prints all the cards the player has.
 * Receives the player whose cards should be printed.
 * Calls for the print card function that will call the right print function, whether the card is a normal card or the card is a special card.
 */
void Print_Player_Cards(PLAYER player)
{
    // For each card the player has.
    for (int card_i = 0; card_i < player.nof_cards; card_i++)
    {
        // Print the card number (which is the index+1 because indexes starts at 0 and our count starts at 1).
        printf("\nCard #%d\n", card_i+1);
        Print_Card(player.cards[card_i]); // Print the card.
    }
}


/*
 * Prints the current top card of the deck, the player's name whose turn it is and all of his cards.
 * Receives the top card and the player whose cards needs printing.
 */
void Print_Current_Deck(CARD top_card, PLAYER player)
{
    // Print the card on top of the deck.
    printf("\nUpper card:\n");
    Print_Card(top_card); // Print the top card.

    // Print the name of the player currently playing.
    printf("\n%s's turn:\n", player.name);

    // Print all the cards the player has.
    Print_Player_Cards(player);
}


/*
 * Prints special card stats line.
 * Receives the stat wanted to be printed.
 */
void Print_Special_Card_Stat(STAT_DATA stat)
{
    int type_i = 0; // The index of the type's string.
    int print_i = COL_LEN/2 - Find_Str_Mid_Index2(stat.card_type); // The index where the type's string needs to start from. Starts from the middle of the collum minus the mid of the card's name.
    int line_i = 0; // The index of the character in the line.

    // While the line index didn't go past the length of the collum.
    while (line_i < COL_LEN)
    {
        // Check if the line index is smaller than the index where the card type will be printed, or the card type was fully printed already.
        if (line_i < print_i || stat.card_type[type_i] == '\0')
            printf(" "); // Print a space.
        else // Print the card type's string.
            {
                printf("%c", stat.card_type[type_i]); // Print the character in the type index.
                type_i++; // Move to the next index in the type's string.
                print_i++; // Move to the next print index.
            }
        line_i++; // Move to the next character in the line.
    }
    // Print the card frequency.
    printf("|    %d\n", stat.card_freq);
}


/*
 * Print the statistics of how many times each card in the game was drawn.
 * Receives the game's data which contains the stats array.
 * The stats array needs to be sorted.
 */
void Print_Game_Stats(GAME_DATA game_data)
{
    // Print title.
    printf("\n************ Game Statistics ************\n");
    printf("Card # | Frequency\n__________________\n");

    // For every stat in the stats array.
    for (int stat_i = 0; stat_i < game_data.nof_stats; stat_i++)
    {
        // Check if the card's type is NORMAL, if not then it's a special card. sends to the right print function accordingly.
        if (!strcmp(game_data.stats[stat_i].card_type, NORMAL))
            printf("   %d   |    %d\n", game_data.stats[stat_i].card_num, game_data.stats[stat_i].card_freq); // Print normal card.
        else
            Print_Special_Card_Stat(game_data.stats[stat_i]); // Print special card.
    }
}


/*
 * Add a new stat that doesn't already exist in the stats array.
 * Receives a pointer to the game's data and the card to be added to the stats array.
 */
void Add_Game_Stat(GAME_DATA* game_data_p, CARD card)
{
    // Copy the card's name into a new stat.
    strcpy(game_data_p->stats[game_data_p->nof_stats].card_type, card.type);

    // Copy the card's number into the new stat.
    game_data_p->stats[game_data_p->nof_stats].card_num = card.num;

    // Add 1 to the card's frequency in the game.
    game_data_p->stats[game_data_p->nof_stats].card_freq++;

    // Add 1 to the number of stats in the stats array.
    game_data_p->nof_stats++;
}


/*
 * Check if a card is in the game statistics,
 * if it is then add 1 to the frequency of that card,
 * if it isn't then adds the card into the game statistics array.
 */
void Check_Stat_Normal_Card(GAME_DATA* game_data_p, CARD card)
{
    int nof_stats = game_data_p->nof_stats; // Get the number of stats in the stats array.
    STAT_DATA* stats_array = game_data_p->stats; // Get a pointer to the stats array.

    // Check every stat in the stats array to find if the card is already in the stats array.
    for(int stat_i = 0; stat_i < nof_stats; stat_i++)
    {
        // Check if the card is already in the stats array.
        if (card.num == stats_array[stat_i].card_num)
        {
            // The card is in the stats array, add 1 to its frequency.
            stats_array[stat_i].card_freq++;
            return; // End the function, found the card.
        }
    }
    // The card isn't in the stats array, add the card to the game stats.
    Add_Game_Stat(game_data_p, card);
}


/*
 * Check if a card is in the game statistics,
 * if it is then add 1 to the frequency of that card,
 * if it isn't then adds the card into the game statistics array.
 */
void Check_Stat_Special_Card(GAME_DATA* game_data_p, CARD card)
{
    int nof_stats = game_data_p->nof_stats; // Get the number of stats in the stats array.
    STAT_DATA* stats_array = game_data_p->stats; // Get a pointer to the stats array.

    // Check every stat in the stats array to find if the card is already in the stats array.
    for(int stat_i = 0; stat_i < nof_stats; stat_i++)
    {
        // Check if the card is already in the stats array.
        if (!strcmp(card.type, stats_array[stat_i].card_type))
        {
            // The card is in the stats array, add 1 to its frequency.
            stats_array[stat_i].card_freq++;
            return; // End the function, found the card.
        }
    }
    // The card isn't in the stats array, add the card to the game stats.
    Add_Game_Stat(game_data_p, card);
}


/*
 * Receives a color number between 0 and 3, returns the correct color char.
 */
char Get_Color_Char(int color_num)
{
    // Check the received color number from 0 to 3. Chose the card's color depending on what number was received.
    switch (color_num)
    {
        case 1: // The color number received is 1.
            return YELLOW; // Set the card's color to YELLOW ('Y')
        case 2: // The color number received is 2.
            return RED; // Set the card's color to RED ('R')
        case 3: // The color number received is 3.
            return BLUE; // Set the card's color to BLUE ('B')
        case 4: // The color number received is 4.
            return GREEN; // Set the card's color to GREEN ('G')
        default:
            printf("ERROR: INVALID NUMBER RECEIVED!"); // If the function received invalid input.

    }
    return ERROR; // If for some reason the function received a wrong input color number, return error value.
}


/*
 * Generates a random color from - (green/red/yellow/blue).
 * The seed for the random function needs to be set in the main function!
 * Returns the result color character. ('G' / 'R' / 'Y' / 'B')
 */
char Get_Random_Color()
{
    int card_color_num = rand() % NUM_OF_COLORS + 1; // Get a random number from 1 to the number of colors in the game (4), which will represent the card's color.
    char card_color_char; // The color's character.

    // Get the character that represents the card's color.
    card_color_char = Get_Color_Char(card_color_num);

    return  card_color_char; // Return the result card color.
}


/*
 * Gets a random normal card with a random card number, a random color, and the card type of NORMAL.
 * The seed for the random function needs to be set in the main function!
 * Receives a pointer to the card's location where the card should be saved.
 */
void Get_Random_Normal_Card(CARD* result_card_p)
{
    int card_num = 1 + rand() % 9; // Get a random card number from 1 to 9, which will be the card's number.
    char card_type[] = NORMAL; // The type of the card is normal. (the function runs for normal cards only)

    // Set the cards' info with the randomized card number and card color, in the result card. The type of card is a normal card.
    strcpy(result_card_p->type, card_type); // Set the result card's type to: NORMAL.
    result_card_p->num = card_num; // Set the result card's number to the random number received.
    result_card_p->color = Get_Random_Color(); // Get a random color and set the result as the card's color.
}


/*
 * Take a random card, can be a normal card and can be one of the special cards.
 * Receives a pointer to the card's location where the random card will be saved.
 */
void Take_Random_Card(CARD* result_card_p)
{
    int card_color = Get_Random_Color(); // Get a random color for the card (green/red/yellow/blue).
    int rnd_card_type_num = rand() % NOF_CARD_TYPES; // Get a random number from 0 to the number of card types (not included), which will represent the card's type.

    result_card_p->color = card_color; // Set the color of the card to the random color received (the COLOR special card will override this value, so it will not contain a color).
    result_card_p->num = EMPTY; // Initialize the card number to be EMPTY (-1), in the special cards the card number is empty and unused (the NORMAL card will override this value with the random card number received).

    // Handle each card type accordingly:
    // The possible card types are: "+" / "STOP" / "<->" / "COLOR" / "TAKI" / "NORMAL"
    switch (rnd_card_type_num)
    {
        // Card type will be "+".
        case 0:
            strcpy(result_card_p->type, PLUS); // Set the result card's type to: +.
            break;

            // Card type will be "STOP".
        case 1:
            strcpy(result_card_p->type, STOP); // Set the result card's type to: STOP.
            break;

            // Card type will be "<->".
        case 2:
            strcpy(result_card_p->type, DIRECTION); // Set the result card's type to: <->.
            break;

            // Card type will be "COLOR".
        case 3:
            strcpy(result_card_p->type, COLOR); // Set the result card's type to: COLOR.
            result_card_p->color = NO_COLOR; // Sets the color of the card to be empty. The player will choose the color when he will play the card.
            break;

            // Card type will be "TAKI".
        case 4:
            strcpy(result_card_p->type, TAKI); // Set the result card's type to: TAKI.
            break;

            // Card type will be "NORMAL".
        case 5:
            strcpy(result_card_p->type, NORMAL); // Set the result card's type to: NORMAL.
            result_card_p->num = 1 + rand() % 9; // Get a random card number from 1 to 9, which will be the card's number.
            break;
    }
}


/*
 *  Hand each player in the players array NOF_START_CARDS (4).
 *  Receives an array of players and its size- the number of players in the game.
 */
void Hand_Start_Cards(GAME_DATA* game_data_p, PLAYER players[], int nof_players)
{
    CARD* current_card_p; // The pointer to the current card being received.

    // For each player in the players array.
    for (int player_i = 0; player_i < nof_players; player_i++)
    {
        players[player_i].nof_cards = 0; // Initialize the number of cards the player has.

        // Add a starting card for this player.
        for (int card_i = 0; card_i < NOF_START_CARDS; card_i++)
        {
            current_card_p = &players[player_i].cards[card_i]; // Get the location of the card in index card_i.
            Take_Random_Card(current_card_p); // Get a random card and insert it into the cards array, in the location of the current card.
            players[player_i].nof_cards++; // Add one to the count of how many cards the player has.

            // Check if the card received is a normal card.
            if (!strcmp(current_card_p->type, NORMAL))
                Check_Stat_Normal_Card(game_data_p, *current_card_p); // Add the normal card to the stats array.
            else // The card is not a normal card, so it's a special card.
                Check_Stat_Special_Card(game_data_p, *current_card_p); // Add the special card to the stats array.
        }
    }
}


/*
 * Reallocates memory of an array of  cards into a new location, with the requested size.
 * Receives a pointer to the player whose cards need reallocation, and the size of the wanted new cards array.
 * Copies every card from the old location into a temporary array of cards, frees the memory previously allocated for the cards array,
 * and tries to reallocate the memory to a new location using malloc, with the size wanted.
 * If the reallocation fails, prints error message and quits the program.
 * If the reallocation succeeded, then copies all the cards from the temp array into the new location, leaving free spots for the new cards.
 */
void Reallocate_Cards_Array(PLAYER* player_p, int size)
{
    int nof_cards = player_p->nof_cards; // Get the current number of cards that the player has.
    CARD tmp_cards[nof_cards]; // A temporary array of cards that the current cards will be copied into.

    // Copy every card that in the cards array into the temp array.
    for (int card_i = 0; card_i < nof_cards; card_i++)
        tmp_cards[card_i] = player_p->cards[card_i]; // Copy the card info into the temp array.

    // Free the memory that the malloc function allocated previously for the cards array (in the old location).
    free(player_p->cards);

    // Try to allocate enough space for the cards array in the new size, in a new location.
    player_p->cards = (CARD*) malloc(sizeof(CARD) * size);

    // Check if the allocation failed.
    if (player_p->cards == NULL)
    {
        printf("Memory allocation failed!!!\n");
        exit(1); // Quit the function.
    }

    // Copy every card from the temp array into the new location that was allocated.
    for (int card_i = 0; card_i < nof_cards; card_i++)
        player_p->cards[card_i] = tmp_cards[card_i]; // Copy the card info into the new location.
}


/*
 * Free the memory allocated by the malloc function for all the cards array.
 * Do it for every player in the players array.
 * Receives array of players and the number of players in the array.
 */
void Free_Cards_Arrays(PLAYER players[], int nof_players)
{
    // For each player in the players array.
    for (int player_i = 0; player_i < nof_players; player_i++)
        free(players[player_i].cards); // Free the memory allocated for the cards array of that player.
}


/*
 * Check if it is possible to drop a normal card on top of the top card.
 * Receives the normal card to be checked and the card at the top of the deck.
 * Returns true if it is possible to drop the wanted normal card,
 * returns false otherwise.
 */
bool Check_Normal_Card(CARD normal_card, CARD top_card)
{
    // Check if the cards' colors are equal.
    if (normal_card.color == top_card.color)
        return true; // The colors are equal, returns true, can drop the card.

    // Check if one of the cards doesn't have a number in it.
    if (normal_card.num == EMPTY || top_card.num == EMPTY)
        return false; // The cards have different colors, and at least one doesn't have a number. Can't drop the card so returns false.

    // Check if the number of the card to check is equal to the number of the top card.
    if (normal_card.num == top_card.num)
        return true; // The cards' numbers are equal, returns true, Can drop the card.

    // The cards' numbers and colors aren't equal, returns false, can't drop the card.
    return false;
}


/*
 * Check if it is possible to drop a special card on top of the top card.
 * Receives the special card to be checked and the card at the top of the deck.
 * Returns true if it is possible to drop the wanted special card,
 * returns false otherwise.
 */
bool Check_Special_Card(CARD special_card, CARD top_card)
{
    // If the card has no color, the card is a color card, and it can be dropped on top of every color.
    if (special_card.color == NO_COLOR)
        return true; // Can drop the card, return true.

    // Check if the cards' colors are equal.
    if (special_card.color == top_card.color)
        return true; // The cards' colors are equal, returns true, can drop the card.

    // Check if the cards' types are equal.
    if (!strcmp(special_card.type, top_card.type))
        return true; // The cards' types are equal, returns true, can drop the card.

    // The card's colors and types aren't equal, returns false, can't drop the card.
    return false;
}


/*
 * Drop a card from the player's cards,
 * Save that card as the new top card,
 * Remove the card from the cards array.
 */
void Remove_Card_From_Array(PLAYER* player_p, int card_i, CARD* top_card_p)
{
    *top_card_p = player_p->cards[card_i]; // Set the top card to the card that is being dropped.

    // Remove the card from the player's cards array. Start at the index of the card being removed. Overwrites the card with the card on the next index, until the end of the cards array.
    for (int i = card_i; i < player_p->nof_cards - 1; i++)
        player_p->cards[i] = player_p->cards[i + 1]; // Move the card from the next index to the current index, overwrite the card on this current index.

    player_p->nof_cards--; // Subtract 1 from the number of cards the player has.
}


/*
 * Draws a new random card for the player.
 * Checks if the card array is full, if it is then reallocates the cards array into a new location with double its size.
 * Then, adds a new random card into the cards array of the player.
 * Receives a pointer of the player that will be drawing a new card.
 */
void Draw_New_Card(GAME_DATA* game_data_p, PLAYER* player_p)
{
    CARD* new_card_p;

    // Check if the cards array is full, then multiples its size by 2 to make new space for new cards.
    if (player_p->nof_cards == player_p->cards_phys_size)
    {
        player_p->cards_phys_size *= 2; // Double the size of the cards array physical size.

        // Try to reallocate the memory of the cards with the new size. If successful, copies the array to the new location and frees the memory in the old location.
        Reallocate_Cards_Array(player_p, player_p->cards_phys_size);
    }
    // Get a pointer to where the new card will be saved (in the index of the nof_card, which will be after the last card in the array).
    new_card_p = &player_p->cards[player_p->nof_cards];

    Take_Random_Card(new_card_p); // Add a new card to the player's cards.
    player_p->nof_cards++; // Add 1 to the number of cards the player has.

    // Add the card into the game stats. Check if the card received is a normal card.
    if (!strcmp(new_card_p->type, NORMAL))
        Check_Stat_Normal_Card(game_data_p, *new_card_p); // Add the normal card to the stats array.
    else // The card is not a normal card, so it's a special card.
        Check_Stat_Special_Card(game_data_p, *new_card_p); // Add the special card to the stats array.
}


/*
 * Tries to drop a normal card on the current top card.
 * Checks if the card's number or color match those of the current top card.
 * If they match, drops the card and returns true.
 * Else, returns false.
 * Receives a pointer to the player, the card's index and a pointer to the top card.
 */
bool Play_Normal_Card(PLAYER* player_p, int card_i, CARD* top_card_p)
{
    // Get the card to play.
    CARD normal_card = player_p->cards[card_i];

    // Check if the card can be dropped on top of the top card.
    if (Check_Normal_Card(normal_card, *top_card_p))
    {
        // Can drop the card on top of the current top card. Drop the card:
        Remove_Card_From_Array(player_p, card_i, top_card_p);
        return true; // The card dropped successfully, Returns true.
    }

    return false; // The card couldn't be played, returns false.
}


/*
 * Lets the player choose what color the card will be.
 * Sets that color as the card's color.
 * Receives a pointer to the player and the color card's index.
 */
void Play_Color_Card(PLAYER* player, int card_i)
{
    int color_choice; // The menu choice for the color of the card.

    // Request a color for the card from the player.
    printf("Please enter your color choice:\n1 - Yellow\n2 - Red\n3 - Blue\n4 - Green\n");
    scanf("%d", &color_choice); // Get the color's number.

    player->cards[card_i].color = Get_Color_Char(color_choice); // Get the character that represents the wanted color.
}


/*
 * Skips the player on the next turn.
 * If the stop card was the last card, and there are only 2 players in the game, then draws a new card for the player and gives the player another turn.
 * Receives a pointer to the game's data and a pointer to the player.
 */
void Play_Stop_Card(GAME_DATA* game_data_p, PLAYER* player_p)
{
    // Check if the stop card is the last card, and that there are 2 players in the game, then the player needs to draw a card.
    if (player_p->nof_cards - 1 == 0 && game_data_p->nof_players == 2)
        Draw_New_Card(game_data_p, player_p); // Draw a new card.

    // Check if the direction is to the right.
    if (game_data_p->is_direction_right)
        game_data_p->player_index++; // Adds 1 to the player index to skip the next player's turn.
    else // The direction is to the left.
        game_data_p->player_index--; // Subtracts 1 from the player index to skip the next player's turn.
}


/*
 * Flips the direction of the game.
 * Receives a pointer to the game's data.
 * Updates the direction variable in the game's data.
 */
void Play_Direction_Card(GAME_DATA* game_data_p)
{
    // Change the direction of the game.
    game_data_p->is_direction_right = !game_data_p->is_direction_right;
}


/*
 * Play TAKI card, enables to play multiple cards of the same color on one turn.
 * Removes the TAKI card from the player's cards array,
 * Lets the player chose to end the turn or place more cards,
 * If the player dropped all his cards, check if the last card dropped was a special card that requires to draw a card,
 * If it wasn't, then the player won the game and ends the function.
 * Receives a pointer to the game's data, a pointer to the player and the index of the TAKI card in the player's cards array.
 */
void Play_Taki_Card(GAME_DATA* game_data_p, PLAYER* player_p, int taki_card_i)
{
    int card_choice, card_index; // The card choice: number from 1 to the number of cards, and the card's index in the cards array.
    CARD chosen_card; // The chosen card's data.

    Remove_Card_From_Array(player_p, taki_card_i, &game_data_p->top_card); // Removes the TAKI card from the cards array of the player.

    while(true) // While the player can drop more cards, until one of the if statements is met.
    {
        // Check if the player dropped all his cards in the TAKI sequence.
        if (player_p->nof_cards == 0)
        {
            // Check if the last card dropped was a PLUS card, so the player needs to draw a card.
            if (!strcmp(game_data_p->top_card.type, PLUS))
            {
                Draw_New_Card(game_data_p, player_p); // Give the player a card.
                return; // Ends the turn.
            }
            // Check if the last card dropped was a STOP card, and the number of players in the game is 2, so the player needs to draw a card.
            if (!strcmp(game_data_p->top_card.type, STOP) && game_data_p->nof_players == 2)
            {
                Draw_New_Card(game_data_p, player_p); // Give the player a card.
                return; // Ends the turn.
            }

            // The last card dropped didn't require to draw a new card, so the player won the game.
            game_data_p->is_game_won = true; // Set the game to be finished.
            return; // Finish the function.
        }
        // Print the current top card, the player's name and all of his cards.
        Print_Current_Deck(game_data_p->top_card, *player_p);

        // Print request message for the player's choice of play. 0 to end the turn or 1 to the number of cards to play that card.
        printf("Please enter 0 if you want to finish your turn\nor 1-%d if you want to put one of your cards in the middle:\n", player_p->nof_cards);
        scanf("%d", &card_choice); // Get the choice input.

        if (card_choice == 0) // Player chose to end his turn.
        {
            // Check if the last card dropped was a special card, if it was, use that card.
            if (!strcmp(game_data_p->top_card.type, PLUS)) // Check if the card was PLUS card.
                Play_Plus_Card(game_data_p, player_p, card_index); // Give the player another turn.
            else
                if (!strcmp(game_data_p->top_card.type, STOP)) // Check if the card was STOP card.
                    Play_Stop_Card(game_data_p, player_p); // Skip the next player's turn.
                else
                    if (!strcmp(game_data_p->top_card.type, DIRECTION)) // Check if the card was DIRECTION card.
                        Play_Direction_Card(game_data_p); // Swap the direction of the game.

            return; // End the function, the sequence is finished.
        }

        // Check if the player entered a wrong input, if so he will be requested for a new input in a new loop sequence.
        if (card_choice >= player_p->nof_cards) { printf("Invalid card! Try again.\n"); continue; }

        card_index = card_choice - 1; // Get the chosen card's index.
        chosen_card = player_p->cards[card_index]; // Get the card chosen.

        // Check if the chosen card's type is color.
        if (!strcmp(chosen_card.type, COLOR))
        {
            // Updates the chosen card's color to be the same color as the current top card's.
            player_p->cards[card_index].color = game_data_p->top_card.color;

            // Remove the chosen card from the cards array of the player. Also update the top card.
            Remove_Card_From_Array(player_p, card_index, &game_data_p->top_card);

            return; // Ends the player's turn, can't put more cards after the color card in TAKI sequence.
        }

        // Check if the chosen card's color isn't the same color as the top card's. Continue to a new loop sequence for a new choice.
        if (chosen_card.color != game_data_p->top_card.color) { printf("Invalid card! Try again.\n"); continue; }

        // Remove the chosen card from the cards array of the player. Also update the top card.
        Remove_Card_From_Array(player_p, card_index, &game_data_p->top_card);
    }
}


/*
 * Gives the player another turn.
 * Removes the PLUS card from the cards array of the player.
 * If the PLUS card was the last card of the player, draws a new card and ends the turn.
 * If it wasn't, move back one turn so when the turn will go to the next player, it will actually go to this player and give him another turn.
 * Receives a pointer to the game's data, a pointer to the player and the card index of the PLUS card.
 */
void Play_Plus_Card(GAME_DATA* game_data_p, PLAYER* player_p, int plus_card_i)
{
    // Removes the card from the cards array of the player. Also update the top card.
    Remove_Card_From_Array(player_p, plus_card_i, &game_data_p->top_card);

    if (player_p->nof_cards == 0) // Check if the PLUS card was the last card of the player.
        Draw_New_Card(game_data_p, player_p); // The PLUS card was the last card, draws a new card.
    else
    {
        // The player still has cards, gives the player another turn (goes 1 index back so when the turn goes to the next player, it will go to this player).
        if (game_data_p->is_direction_right)
            game_data_p->player_index--; // The game's direction is to the right, so goes to the left index.
        else
            game_data_p->player_index++; // The game's direction is to the left, so goes to the right index.
    }
}


/*
 * Checks if the player dropped all his cards,
 * Prints the winners name and ends the program. The game is finished.
 * Returns true if the player won, and false if he didn't.
 * Receives player struct to be checked.
 */
bool Check_Winner(PLAYER player)
{
    // Check if the player dropped all of his cards, if so then the game is finished and the player has won.
    if (player.nof_cards == 0)
    {
        // Game Finished!!!
        // Print the finished game message with the winner's name.
        printf("\nThe winner is... %s! Congratulations!\n", player.name);

        return true; // The player has won, returns true.
    }
    return false; // The player didn't win, returns false.
}


/*
 * Sort the stats array in the game's data by the frequency of the cards.
 * The card that was drawn most times will be at the first index, with descending order for the other cards' frequencies.
 * Sorts by the bubble sort method, pushes the stats with the lower frequencies to the right, until the array is sorted.
 * Receives pointer to the game's data.
 */
void Sort_Stats_Array(GAME_DATA* game_data_p)
{
    int i, j; // The indexes as mentioned later.
    STAT_DATA* stats_array = game_data_p->stats; // Get the array containing the stats.
    int nof_stats = game_data_p->nof_stats; // Get the number of stats in the array.

    // i = the limit on the right of the array, after the limit the sorted stats will take their spots.
    // j = the current stat index where we check if the stat's frequency is lower than the one that comes after it.
    for (i = 0; i < nof_stats - 1; i++)
        for (j = 0; j < nof_stats - i - 1; j++)
            if (stats_array[j].card_freq < stats_array[j + 1].card_freq) // Check if the stat freq in the current index is lower than the stat freq in the next index.
                Swap_Stats(stats_array, j, j + 1); // Swaps the stats so the one with the lower freq will move to the right.
}


/*
 * Swaps two stats in array of stats.
 * Receives array of stats and the indexes of the stats to swap.
 */
void Swap_Stats(STAT_DATA stats[], int first_i, int second_i) {
    STAT_DATA tmp_stat = stats[first_i]; // Get the value of the stat in the first index into a temp variable, because the first index value is later overwritten.
    stats[first_i] = stats[second_i]; // Set the stat at the first index as the stat in the second index.
    stats[second_i] = tmp_stat; // Set the stat from the temp variable into the second index.
}


/*
 * Try to play a card given.
 * Checks if it's possible to drop the card on top of the current top card. If it's not then returns false.
 * If it's possible to drop the card, calls for the right play card function that will player the card.
 * Receives a pointer to the game's data, a pointer to the player and the index of the card in the player's cards array.
 * Both TAKI and PLUS cards gives the player more turns, so they are handled differently, the cards needs to be removed in their functions, and then receive new cards.
 */
bool Try_Play_Card(GAME_DATA* game_data_p, PLAYER* player_p, int card_i)
{
    CARD current_card = player_p->cards[card_i]; // Get the card that is being dropped.

    // ------------------- Normal Card -------------------
    // Check if the card type is: "NORMAL".
    if (!strcmp(current_card.type, NORMAL))
        return Play_Normal_Card(player_p, card_i, &game_data_p->top_card); // Try to drop the card, returns true if was successful, false if wasn't.

    // ------------------- Special Cards -------------------
    // cards types: "+" / "STOP" / "<->" / "COLOR" / "TAKI"

    // The card wasn't a normal card, check if the special card can be dropped on top of the top card.
    if (!Check_Special_Card(current_card, game_data_p->top_card))
        return false; // Couldn't play the special card, returns false.

    // The card can be dropped, checks the card's type.
    // Check if the card type is: "STOP".
    if (!strcmp(current_card.type, STOP))
        Play_Stop_Card(game_data_p, player_p); // Skip the next player's turn.
    else
        // Check if the card type is: "<->".
        if (!strcmp(current_card.type, DIRECTION))
            Play_Direction_Card(game_data_p); // Change the direction of the game.
        else
            // Check if the card type is: "COLOR".
            if (!strcmp(current_card.type, COLOR))
                Play_Color_Card(player_p, card_i); // Let the player chose the color of the card.
            else
                // Check if the card type is: "TAKI"".
                if (!strcmp(current_card.type, TAKI))
                {
                    Play_Taki_Card(game_data_p, player_p, card_i); // Play the TAKI card, also removes the card from the array.
                    return true; // Finished dropping the card.
                }
                else
                    // Check if the card type is: "+".
                    if (!strcmp(current_card.type, PLUS))
                    {
                        Play_Plus_Card(game_data_p, player_p, card_i); // Play the PLUS card, also removes the card from the array.
                        return true; // Finished dropping the card.
                    }

    // Removes the card from the cards array of the player. Also update the top card.
    // Note that the TAKI and PLUS cards were already removed and left the function before this line.
    Remove_Card_From_Array(player_p, card_i, &game_data_p->top_card);
    return true; // Returns true, the card was dropped.
}


/*
 * Start playing the game.
 * Receives a pointer to the game's data.
 */
void Play_Game(GAME_DATA* game_data_p)
{
    PLAYER* current_player_p; // A pointer to the player that is currently playing.
    PLAYER current_player; // The player that is currently playing.
    int card_chosen; // The number of the card wished to be played. If 0, then draw a new card.
    bool is_play_successful; // If a card was successfully dropped.

    // While no player has dropped all his cards.
    while(!game_data_p->is_game_won)
    {
        // Check if finished a full revolution of turns, every player played his turn, so restarts to the first/last player according to the game's direction. If a stop card was played, then it will skip the turn of that first/last player.
        if (game_data_p->is_direction_right && game_data_p->player_index >= game_data_p->nof_players)
            game_data_p->player_index -= game_data_p->nof_players; // Game direction is to the right, then subtracts the number of players from the index to start from the first player.
        else
            if (!game_data_p->is_direction_right && game_data_p->player_index <= -1)
                game_data_p->player_index += game_data_p->nof_players; // Game direction is to the left, then adds the number of players to the index to start from the last player.

        // ------------------ Each player gets to play his turn: ------------------
        current_player_p = &game_data_p->players[game_data_p->player_index]; // Get a pointer to the data of the player that is currently playing.
        current_player = *current_player_p; // Get the values of the player's data.

        // Print the current top card, the player's name and the player's cards.
        Print_Current_Deck(game_data_p->top_card, current_player);

        // Until the player entered a valid input, keeps requesting for a card choice.
        while (true)
        {
            // Print request message for what play the player wants to do. 0: Draw a card from the deck, 1 to number of cards: Drop a card the player has.
            printf("Please enter 0 if you want to take a card from the deck\nor 1-%d if you want to put one of your cards in the middle:\n", current_player.nof_cards);
            scanf("%d", &card_chosen); // Get the input for the card chosen.

            // If the player chose to draw a new card.
            if (card_chosen == 0)
            {
                Draw_New_Card(game_data_p, current_player_p); // Draw a random card, reallocates the memory of the cards array to fit the new card.
                break; // Finished the turn.
            }

            // If the player wishes to drop a card that he has (player chose a number from 1- the first card, to the last card- the number of cards).
            if (1 <= card_chosen <= current_player.nof_cards)
            {
                // Try to play the card, receive if the action was successful.
                is_play_successful = Try_Play_Card(game_data_p, current_player_p, card_chosen - 1); // Play the card in the correct index of the array of cards- [card chosen number - 1] (the indexes start at 0 while our count starts at 1).

                // If the card was successfully dropped.
                if (is_play_successful)
                {
                    // Check if the player won the game.
                    if (Check_Winner(*current_player_p))
                    {
                        game_data_p->is_game_won = true; // The game is finished, the player won the game.
                        return; // Finish the game, return to the main function.
                    }
                    break; // Stop the loop, a card was dropped and the turn is finished.
                }
            }

            // A card wasn't dropped, the player entered a wrong input and will be requested for a new input in a new loop sequence.
            printf("Invalid card! Try again.\n"); // Print wrong input message.
        }

        // Check the direction of the game and move the index accordingly.
        if (game_data_p->is_direction_right)
            game_data_p->player_index++; // Move index to the right.
        else
            game_data_p->player_index--; // Move index to the left.
    }
}