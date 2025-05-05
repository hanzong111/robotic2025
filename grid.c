#include "robot.h"

void grid_init(Grid *grid) {
    // Initialize all cells to '0'
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            grid->cells[i][j] = '0';
        }
    }
    
    // Set row 4 to 'D' (drop-off points)
    for (int j = 0; j < COLS; j++) {
        grid->cells[4][j] = 'D';
    }

    // Set all ore positions to -1
    for (int k = 0; k < 4; k++) {
        grid->blue[k].x = -1;
        grid->blue[k].y = -1;
        grid->red[k].x = -1;
        grid->red[k].y = -1;
    }
}

void get_positions(Grid *grid, int *Red_Ores, int *Blue_Ores)
{
    for (int i = 0; i < 4; i++)
    {
        int pos_red = Red_Ores[i];
        int pos_blue = Blue_Ores[i];
        // Convert position to grid coordinates (rows 1-3)
        // Assuming positions are 1-indexed (1-15)
        int red_row = ((pos_red - 1) / COLS) + 1;
        int red_col = (pos_red - 1) % COLS;

        int blue_row = ((pos_blue - 1) / COLS) + 1;
        int blue_col = (pos_blue - 1) % COLS;

        grid->red[i].x = red_col;
        grid->red[i].y = red_row;
        grid->cells[red_row][red_col] = 'R';  // Mark red ore on grid
        grid->blue[i].x = blue_col;
        grid->blue[i].y = blue_row;
        grid->cells[blue_row][blue_col] = 'B';  // Mark blue ore on grid
    }
}