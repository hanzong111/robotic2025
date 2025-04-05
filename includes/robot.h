#ifndef ROBOT_H
#define ROBOT_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

#define ROWS 5          // 5 rows (0-4)
#define COLS 5          // 5 columns
#define ORE_COUNT 4     // Number of red/blue ores
#define EMPTY '\0'      // Empty cell
#define BASKET_SIZE 4   // Robot basket size

#define ANGLE_EPSILON 0.001f
#define PI M_PI

#define RED "\033[0;91mR "
#define BLUE "\033[0;94mB "
#define GREEN "\033[0;92mD "

typedef struct {
    int x;
    int y;
} Position;

typedef enum {
    STOP = 0,
    MOVE = 1,
    TURN_RIGHT = 2,
    TURN_LEFT = 3,
    GRAB = 4,
    PUT = 5,
} Chassis_Action;

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
    Position        pos;
    Position        dir;
    Position        facing;
    Position        old_pos;
    char            basket[4];
    Chassis_Action  instructions[100];
    Node            path_compare[4];
} Robot;

// Predefined direction vectors (compile-time constants)
extern const Position DIRECTION_VECTORS[NUM_DIRECTIONS];

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
void    robot_facing(Robot *robot, Direction dir);

#endif