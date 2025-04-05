#ifndef A_STAR_H
#define A_STAR_H

#include "robot.h"

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

Position get_direction(Direction dir);

//A*.c
int     heuristic(Position a, Position b);
void    a_star_init(path *p);
void    a_star(path *p, Position start, Position goal, Grid *grid);

//open_list_utils.c
bool    open_list_is_empty(int open_size);
int     open_list_find(Position pos, int open_size, Node *open_list);
void    open_list_push(Node node, path *p);
Node    open_list_pop(path *p);

//convert_path_to_actions
int     convert_path_to_actions(const Position *path, int path_length, Robot *robot);
float   angle_between_vectors(const Position *a, const Position *b);
float   vector_to_angle(const Position *dir);
void    add_turns(Robot *r, Direction dir, int *instruction_count);
#endif