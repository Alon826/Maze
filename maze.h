#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct 
{
    unsigned playeri;
    unsigned playerj;
} location_t;

typedef struct 
{
    unsigned maze_cols;
    unsigned maze_rows;
    char *message;
    char **two_d;
    unsigned targets;
    int portals[20][3];
} maze_t;
#define MAX_NAME 256

#define LEN(arr)    ((int) (sizeof (arr) / sizeof (arr)[0]))
#define MSG_NORMAL  "Press A/S/D/W to move, Q to quit"
#define MSG_LOSE    "Boom! You lose..."
#define MSG_HOLLOW  "This wall sounds hollow..."
#define MSG_PORTAL  "Magic!"
#define MSG_START   msg_start[rand() % LEN(msg_start)]
#define MSG_UNKNOWN msg_unknown[rand() % LEN(msg_unknown)]
#define MSG_EXIT    msg_quit[rand() % LEN(msg_quit)]
#define MSG_WALL    msg_wall[rand() % LEN(msg_wall)]
#define MSG_WIN     msg_win[rand() % LEN(msg_win)]

void set_terminal(bool start);                                          
void print_maze(maze_t *maze, location_t *location);                   
void first_last_line(int length);                                      //prints the first and last line of the maze
void handle_input(maze_t *maze, location_t *location, bool game_over); 
void handle_input_tiles(maze_t *maze, location_t *location);
void build_maze(maze_t *maze);                                         //allocates memory to the maze
void free_maze(maze_t *maze);                                          //free the memory of the maze
void print_tile(char c, char *color);                                  //prints tiles with color
maze_t * read_maze(char *file, maze_t *maze, location_t *location);    //reads text maze
int portal_check(char c);                                              //checks if the tile is a portal

/************************************
 * TERMINAL CODES ("MAGIC STRINGS") *
 ************************************/

#define COLOR_OFF       "\e[0m"
#define COLOR_BLACK     "\e[0;30m"
#define COLOR_GRAY      "\e[1;30m"
#define COLOR_RED       "\e[0;31m"
#define COLOR_LT_RED    "\e[1;31m"
#define COLOR_GREEN     "\e[0;32m"
#define COLOR_LT_GREEN  "\e[1;32m"
#define COLOR_BROWN     "\e[0;33m"
#define COLOR_YELLOW    "\e[1;33m"
#define COLOR_BLUE      "\e[0;34m"
#define COLOR_LT_BLUE   "\e[1;34m"
#define COLOR_PURPLE    "\e[0;35m"
#define COLOR_LT_PURPLE "\e[1;35m"
#define COLOR_CYAN      "\e[0;36m"
#define COLOR_LT_CYAN   "\e[1;36m"
#define COLOR_LT_GRAY   "\e[0;37m"
#define COLOR_WHITE     "\e[1;37m"

#define CLEAR_SCREEN    "\e[2J\e[1;1H"

/*****************************
 * SYMBOLS FOR GAME ELEMENTS *
 *****************************/

#define S_FLOOR     ' '
#define S_PLAYER    '@' 
#define S_WALL      '#' 
#define S_TARGET    '$' 
#define S_SIDE      '-'
#define S_UP_DOWN   '|'
#define S_TRAP      '!'
#define S_HOLLOW    '?'
#define S_V_TARGET  'v'
#define S_PORTAL    'd'
