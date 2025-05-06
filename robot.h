#ifndef ROBOT_H
#define ROBOT_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
# include <BFS.h>

#define ROWS 5          // 5 rows (0-4)
#define COLS 5          // 5 columns
#define ORE_COUNT 4     // Number of red/blue ores
#define EMPTY '\0'      // Empty cell
#define BASKET_SIZE 4   // Robot basket size

// Ensure these are properly defined
#define ANGLE_EPSILON 1e-6  // ~0.000057 degrees
#define M_PI 3.14159265358979323846  // If not already provided by math.h

#define RED "\033[0;91mR "
#define BLUE "\033[0;94mB "
#define GREEN "\033[0;92mD "

typedef struct {
    int x;
    int y;
} Position;

typedef enum {
    STOP = 0,
    MOVE_FOWARD = 1,
    MOVE_BACKWARD = 2,
    TURN_RIGHT = 3,
    TURN_LEFT = 4,
    GRAB_RED = 5,
    GRAB_BLUE = 6,
    DROP = 7,
    END = 8,
    U_TURN = 9,

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
extern Chassis_Action* Task_List; 

//grid.c
void    grid_init(Grid *);
void get_positions(Grid *grid, FireSource *fire_source);

//robot.c
void    robot_init(Robot *robot);
void    robot_facing(Robot *robot, Direction dir);
Chassis_Action* get_Task_List(void);

int convert_path_to_actions(const Point *path, int path_length, Robot *r, Grid *grid);
float angle_between_vectors(const Position *a, const Position *b);

void    Path_Planning();

void    add_dropoff_path();

#endif