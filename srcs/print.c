#include "../includes/robot.h"

void print_grid(const Grid *grid) {
    printf("\n");
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            char c = grid->cells[i][j];
            if (c == 'R') printf(RED);      // Red
            else if (c == 'B') printf(BLUE);  // Blue
            else if (c == 'D') printf(GREEN);  // Green drop-off
            else printf("\033[0;97m%c ", c);            // White
        }
        printf("\033[0m\n");
    }
}

void print_ore_positions(const Grid *map) {
    printf("Blue positions:\n");
    for (int i = 0; i < ORE_COUNT; i++) {
        printf("  Ore %d: (%d, %d)\n", i, map->blue[i].x, map->blue[i].y);
    }
    
    printf("\nRed positions:\n");
    for (int i = 0; i < ORE_COUNT; i++) {
        printf("  Ore %d: (%d, %d)\n", i, map->red[i].x, map->red[i].y);
    }
}