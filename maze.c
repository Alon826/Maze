#include "maze.h"
char *msg_quit[] = {"Bye!", "Farewell!", "Goodbye!"};
char *msg_start[] = {"Let's start! ", "Go!!! ", "Welcome! ", "Ten barosh!! "};
char *msg_unknown[] = {"Wrong button!", "Try again!", "What?! press the right keys"};
char *msg_wall[] = {"Ouch!!", "You hit the wall!"};
char *msg_win[] = {"You win!!", "You did it!!", "Fantastic!!"};

int main(int argc, char **argv)
{
    time_t t;
    srand((unsigned) time(&t));                // set random
    set_terminal(true);
    location_t new_location = {0};
    maze_t new_maze = {0};
    maze_t *maze;
    location_t *location;
    maze = &new_maze;
    location = &new_location;
    maze = read_maze(argv[1], maze, location); //read the text maze
    print_maze(maze, location);                //print the maze
    bool game_over = false;
    handle_input(maze, location, game_over);   //handle the input
    set_terminal(false);
    free_maze(maze);                           //free maze
    return 0;
}
#define puts_no_newline(s) fputs(s, stdout)

void print_maze(maze_t *maze, location_t *location)
{
    char c;
    char msg[MAX_NAME];
    strcpy(msg, COLOR_WHITE);
    strcat(msg, maze->message);
    static unsigned frame_num; // this variable retains its value between calls
    // clear terminal
    puts_no_newline(CLEAR_SCREEN);
    // print frame number and message
    printf("%-*.*s%11d\n", maze->maze_cols-2, maze->maze_cols-2, msg, frame_num++);
    // print header row
    first_last_line(maze->maze_cols + 2);
    // print maze rows (including player)
    for (int i = 0; i < maze->maze_rows; i++) 
    {
        print_tile(S_WALL, COLOR_GRAY);
        for (int j = 0; j < maze->maze_cols; j++)
        {
            if(i == location->playeri && j == location->playerj)  //handle the tile of the player
            {
                if(S_SIDE == maze->two_d[i][j])                   //"-"
                    print_tile(S_SIDE, COLOR_BLUE);               
                else if(S_UP_DOWN == maze->two_d[i][j])           //"|"
                    print_tile(S_UP_DOWN, COLOR_BLUE);
                else if(S_HOLLOW == maze->two_d[i][j])            //"?"
                    print_tile(S_WALL, COLOR_BLUE);
                else if(S_TRAP == maze->two_d[i][j])              //"!"
                    print_tile(S_WALL, COLOR_BLUE);
                else if(S_V_TARGET == maze->two_d[i][j])          //achived target
                    print_tile(S_TARGET, COLOR_BLUE);
                else if (portal_check(maze->two_d[i][j]) > -1)    //"d"
                    print_tile(S_PORTAL, COLOR_BLUE);
                else
                    print_tile(S_PLAYER, COLOR_BLUE);             //"@"
            }
            else                                                  //handle the other tiles
            {
                c = maze->two_d[i][j];
                if(S_SIDE == c)
                    print_tile(c, COLOR_GRAY);
                else if(S_UP_DOWN == c)
                    print_tile(c, COLOR_GRAY);
                else if(S_HOLLOW == c)
                    print_tile(S_WALL, COLOR_GRAY);
                else if(S_TRAP == c)
                    print_tile(S_WALL, COLOR_GRAY);
                else if(S_WALL == c)
                    print_tile(S_WALL, COLOR_GRAY);
                else if(S_PLAYER == c)
                    print_tile(S_FLOOR, COLOR_GRAY);    
                else if(S_TARGET == c)
                    print_tile(S_TARGET, COLOR_RED);
                else if(S_V_TARGET == c)
                    print_tile(S_TARGET, COLOR_YELLOW);
                else if(portal_check(c) > -1)
                    print_tile(S_PORTAL, COLOR_GREEN);
                else
                    print_tile(c, COLOR_GRAY);
            }
        }
        print_tile(S_WALL, COLOR_GRAY);
        putchar('\n');
    }
    // print footer row
    first_last_line(maze->maze_cols + 2);
}

void first_last_line(int length)
{
    for (int j = 0; j < length; j++)
        print_tile(S_WALL, COLOR_GRAY);
    putchar('\n');
}

void handle_input(maze_t *maze, location_t *location, bool game_over)
{
    do {
        maze->message = MSG_NORMAL;
        bool unknown = false;
        // get user input
        int key = getchar();
        switch(key) {
            case EOF:
            case 'q':
            case 'Q':
                maze->message = MSG_EXIT;   // exit game
                game_over = true;
                break;
            case 'a':
            case 'A':   // go left
                if(0 != location->playerj && S_WALL != maze->two_d[location->playeri][location->playerj-1]                      //handle "#"
                                          && S_UP_DOWN != maze->two_d[location->playeri][location->playerj-1])                  //handle "|"
                    location->playerj -= 1;
                else
                    maze->message = MSG_WALL;
                break;
            case 'd':
            case 'D':   // go right
                if(location->playerj+1 != maze->maze_cols && S_WALL != maze->two_d[location->playeri][location->playerj+1]     //handle "#"
                                                          && S_UP_DOWN != maze->two_d[location->playeri][location->playerj+1]) //handle "|"
                    location->playerj += 1;
                else
                    maze->message = MSG_WALL;
                break;
            case 'w':
            case 'W':   // go up
                if(0 != location->playeri && S_WALL != maze->two_d[location->playeri-1][location->playerj]                    //handle "#"
                                          && S_SIDE != maze->two_d[location->playeri-1][location->playerj])                   //handle "-"
                    location->playeri -= 1;
                else
                    maze->message = MSG_WALL;
                break;
            case 's':
            case 'S':   // go down
                if(location->playeri+1 != maze->maze_rows && S_WALL != maze->two_d[location->playeri+1][location->playerj]    //handle "#"
                                                          && S_SIDE != maze->two_d[location->playeri+1][location->playerj])   //handle "-"
                    location->playeri += 1;
                else
                    maze->message = MSG_WALL;
                break;
            default:
            {
                maze->message = MSG_UNKNOWN;
                unknown = true;
            }
        }
        if(game_over != true && unknown != true)  //handle messeges and and uniqe tiles
        {       
            if(S_TARGET == maze->two_d[location->playeri][location->playerj] && 1 == maze->targets)
            {
                maze->message = MSG_WIN;
                game_over = true;
            }
            if(S_TRAP == maze->two_d[location->playeri][location->playerj])
            {
                maze->message = MSG_LOSE;
                game_over = true; 
            }
            handle_input_tiles(maze, location);
        }
        // print maze
        print_maze(maze, location);
    } while (!game_over);
}
void build_maze(maze_t *maze)
{
    maze->two_d = malloc(maze->maze_rows * sizeof(char *)); 
    if(NULL == maze->two_d)
    {
        printf("Fatal error: memory allocation failed!\n");
        exit(1);
    }
    for (int i = 0; i < maze->maze_rows; i++)
    {
        maze->two_d[i] = NULL;
    }
    for (int i = 0; i < maze->maze_rows; i++)
    {
        maze->two_d[i] = malloc(maze->maze_cols * sizeof(char));
        if(NULL == maze->two_d[i])
        {
            free_maze(maze);
            printf("Fatal error: memory allocation failed!\n");
            exit(1);
        }
    }
}

void free_maze(maze_t *maze)
{
    for (int i = 0; i < maze->maze_rows; i++)
    {
        free(maze->two_d[i]);
    }
    free(maze->two_d); 
}

void print_tile(char c, char *color)
{
    int len_color = 0;
    len_color = strlen(color);
    char s[len_color + 2];
    strcpy(s, color);
    s[len_color] = c;
    s[len_color+1] = '\0';
    puts_no_newline(s);
}

maze_t * read_maze(char *file, maze_t *maze, location_t *location)
{
    char str_name[MAX_NAME]; 
    char str_size[MAX_NAME];
    int rows = 0;
    int cols = 0;
    int k = 0;
    bool player = false;     //make sure there's only one player tile
    FILE *f;
    f = fopen(file, "r");
    if(NULL == f)
    {
        exit(1);
    }
    fgets(str_name, MAX_NAME, f);
    fgets(str_size, MAX_NAME, f);
    for (int i = 0; i < strlen(str_name); i++)          //reads first line
    {
        if('\r' == str_name[i] || '\n' == str_name[i])
        {
            str_name[i] = '\0';
            break;
        }
    }
    maze->message = str_name;
    for (k = 0; ' ' != str_size[k]; k++)                //reads second line
    {
        rows = rows * 10 + (str_size[k] - '0');
    }
    k++;
    for(k=k; '\r' != str_size[k] && '\n' != str_size[k]; k++)
    {
        cols = cols * 10 + (str_size[k] - '0');
    }
    maze->maze_rows = rows;
    maze->maze_cols = cols;
    char str[cols+2];
    build_maze(maze);
    for (int i = 0; i < rows; i++)                     //reads all the maze
    {
        if(NULL != fgets(str, cols+4, f))
        for (int j = 0; '\r' != str[j] && EOF != str[j] && '\n' != str[j] && '\0' != str[j]; j++)
        {
            if(S_PLAYER == str[j] && false == player)
            {
                location->playeri = i;
                location->playerj = j;
                maze->two_d[i][j] = S_PLAYER;
                player = true;
            }
            else if(S_TARGET == str[j])
            {
                maze->two_d[i][j] = S_TARGET;
                maze->targets += 1;
            }
            else if(S_WALL == str[j])
                maze->two_d[i][j] = S_WALL;
            else if(S_SIDE == str[j])
                maze->two_d[i][j] = S_SIDE;
            else if(S_UP_DOWN == str[j])
                maze->two_d[i][j] = S_UP_DOWN;
            else if(S_FLOOR == str[j])
                maze->two_d[i][j] = S_FLOOR;
            else if(S_TRAP == str[j])
                maze->two_d[i][j] = S_TRAP;
            else if(S_HOLLOW == str[j])        
                maze->two_d[i][j] = S_HOLLOW;
            else if(portal_check(str[j]) > -1) // portal insert
            {
                int num = portal_check(str[j]);
                maze->two_d[i][j] = str[j];
                if(1 != maze->portals[num][2])
                {
                    maze->portals[num][0] = i;
                    maze->portals[num][1] = j;
                    maze->portals[num][2] = 1;
                }
                else
                {
                    num += 10;
                    maze->portals[num][0] = i;
                    maze->portals[num][1] = j;
                    maze->portals[num][2] = 2;
                }
            }
            else
            {
                free_maze(maze);
                printf("Error: The maze is not readable\n");
                exit(1);
            }    
        }   
    }
    fclose(f);
    return maze;
}

int portal_check(char c)
{
    for (int i = 0; i < 10; i++)
    {
        if(c == (i + '0'))
            return i;
    }
    return -1;
}

void handle_input_tiles(maze_t *maze, location_t *location)
{
    if(S_HOLLOW == maze->two_d[location->playeri][location->playerj])
    {
        maze->message = MSG_HOLLOW;
    }
    if(S_TARGET == maze->two_d[location->playeri][location->playerj])
    {
        maze->targets -= 1;
        maze->two_d[location->playeri][location->playerj] = S_V_TARGET;
    }
    if(portal_check(maze->two_d[location->playeri][location->playerj]) > -1)
    {
        int p = portal_check(maze->two_d[location->playeri][location->playerj]);
        if(location->playeri == maze->portals[p][0] && location->playerj == maze->portals[p][1])
        {
            location->playeri = maze->portals[p+10][0];
            location->playerj = maze->portals[p+10][1];
        }
        else
        {
            location->playeri = maze->portals[p][0];
            location->playerj = maze->portals[p][1];
        }
        maze->message = MSG_PORTAL;
    }
}