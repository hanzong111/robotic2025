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

void get_positions(Grid *grid, FireSource *fire_source)
{
    int j = 0;
    int k = 0;

    for (int i = 0; i < 8; i++)
    {
        if (fire_source->color == 0) //RED
        {
            grid->red[j].x = fire_source->col - 1;
            grid->red[j].y = fire_source->row;
            grid->cells[grid->red[i].y][grid->red[i].x] = 'R'; // Mark blue ore on the grid
            j++;
        }
        else if (fire_source->color == 1) //BLUE
        {
            grid->blue[k].x = fire_source->col - 1;
            grid->blue[k].y = fire_source->row;
            grid->cells[grid->blue[i].y][grid->blue[i].x] = 'B'; // Mark blue ore on the grid
            k++;
        }
    }
}