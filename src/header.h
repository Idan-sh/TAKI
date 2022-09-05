#ifndef HEADER_H // Include header.h only once.
#define HEADER_H

// ---------- Include ---------

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

// ---------- Define ----------

#define MAX_NAME_LEN 21 // The maximum length of the first name of each player. the maximum length is 20 charaters.
#define MAX_CARD_NAME_LEN 7 // The maximum length of the special card's type string. (The longest string is 'NORMAL' which contains 7 characters)

// Dimensions of each card:
#define CARD_LENGTH 6
#define CARD_WIDTH 9
#define CARD_BORDER "*" // The character for the border of the cards

#define NOF_START_CARDS 4 // How many cards each player starts the game with.

#define EMPTY -1 // Empty card number will be represented by -1, so the card number value in the special cards will be initialized to -1.

// Characters for the cards' colors:  (green/red/yellow/blue).
#define GREEN 'G'
#define RED 'R'
#define YELLOW 'Y'
#define BLUE 'B'
#define NO_COLOR ' '
#define ERROR 'E'
#define NUM_OF_COLORS 4 // The number of card colors in the game.

// Types of cards: "+" / "STOP" / "<->" / "COLOR" / "TAKI" / "NORMAL".
#define PLUS "+"
#define STOP "STOP"
#define DIRECTION "<->"
#define COLOR "COLOR"
#define TAKI "TAKI"
#define NORMAL "NORMAL"
#define NOF_CARD_TYPES 6 // Number of card types in the game.

// Stats 
#define GAME_STATS_MAX_SIZE 14 // The possible stats are for 9 number cards and 5 special cards, 14 in total.
#define STAT_EMPTY "EMPTY" // Indicates that the stat is empty.
#define COL_LEN 7 // The length of the stats collum.


// ---------- Data Stractures ----------

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

void Print_Special_Card_Stat(STAT_DATA stat);

void Print_Game_Stats(GAME_DATA game_data);

// ------------------ Game Setup Functions --------------------

void Set_Nof_Players(int* nof_players);

void Set_Players_Names(PLAYER players_data[], int size);

void Init_Game_Data(GAME_DATA* game_data_p);

void Hand_Start_Cards(GAME_DATA* game_data_p, PLAYER players[], int nof_players);

// ---------- Allocation and Initialization Functions ----------

void Init_Allocate_Players(GAME_DATA* game_data_p);

void Init_Allocate_Players_Cards(GAME_DATA* game_data_p, int size);

void Reallocate_Cards_Array(PLAYER* player_p, int size);

void Free_Cards_Arrays(PLAYER players[], int nof_players);

// -------------------- Randomize Functions ---------------------

void Take_Random_Card(CARD* result_card_p);

void Get_Random_Normal_Card(CARD* result_card_p);

char Get_Random_Color();

char Get_Color_Char(int color_num);

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

#endif // HEADER_H end if.