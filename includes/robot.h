#ifndef ROBOT_H
#define ROBOT_H

#include <stdio.h>
#include <stdbool.h>

#define ROWS 5       // 5 rows (0-4)
#define COLS 5       // 5 columns
#define ORE_COUNT 4  // Number of red/blue ores

#define RED "\033[0;91mR "
#define BLUE "\033[0;94mB "
#define GREEN "\033[0;92mD "

typedef struct {
    char cells[ROWS][COLS];
} Grid;

void print_grid(const Grid *);
void initialize_grid(Grid *);
void get_positions(Grid *grid, char ore_type);

#endif