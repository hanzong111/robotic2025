#ifndef ROBOT_H
#define ROBOT_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#define ROWS 5          // 5 rows (0-4)
#define COLS 5          // 5 columns
#define ORE_COUNT 4     // Number of red/blue ores
#define EMPTY '\0'      // Empty cell
#define BASKET_SIZE 4   // Robot basket size

#define RED "\033[0;91mR "
#define BLUE "\033[0;94mB "
#define GREEN "\033[0;92mD "

typedef struct {
    int x;
    int y;
} Position;

typedef enum {
    LEFT,
    RIGHT, 
    UP,
    DOWN,
    NUM_DIRECTIONS
} Direction;

typedef struct {
    char cells[ROWS][COLS];
    Position blue[ORE_COUNT];
    Position red[ORE_COUNT];
} Grid;

typedef struct {
    Position pos;
    int f;  // f(n) = g(n) + h(n)
} Node;

typedef struct {
    Position pos;
    Position dir;
    Position old_pos;
    char     basket[4];
    char     instructions[25][6];
    Node     path_compare[4];
} Robot;


typedef struct {
    Node        open_list[ROWS * COLS];
    Position    came_from[ROWS][COLS];
    Position    path[ROWS * COLS];
    int         g_score[ROWS][COLS];
    int         open_size;
    int         path_length;
    bool        closed_list[ROWS][COLS];
    bool        is_target;
} path;

// Predefined direction vectors (compile-time constants)
extern const Position DIRECTION_VECTORS[NUM_DIRECTIONS];
Position get_direction(Direction dir);

//grid.c
void    grid_init(Grid *);
void    get_positions(Grid *grid, char ore_type);
void    copy_grid(char (*dest)[5][5], char (*src)[5][5]);
bool    update_map(Grid *grid, int x, int y, char new_char);

//print.c
void    print_grid(const Grid *);
void    print_ore_positions(const Grid *grid);

//robot.c
void    robot_init(Robot *robot);
void    update_robot_position(Robot *robot, int x, int y);
bool    update_robot_basket(Robot *robot, int index, char value);
int     find_from_basket(const Robot *robot, char value);
void    robot_turn(Robot *robot, Direction dir);

//A*.c
int     heuristic(Position a, Position b);
void    a_star_init(path *p);
void    a_star(path *p, Position start, Position goal, Grid *grid);
int     convert_path_to_instructions(const Position *path, int path_length, Robot *robot);

//open_list_utils.c
bool    open_list_is_empty(int open_size);
int     open_list_find(Position pos, int open_size, Node *open_list);
void    open_list_push(Node node, path *p);
Node    open_list_pop(path *p);

#endif