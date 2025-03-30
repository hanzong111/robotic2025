#include "../includes/robot.h"

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

void get_positions(Grid *grid, char ore_type) {
    printf("\nEnter %d %s ore positions (1-15):\n", ORE_COUNT, 
           (ore_type == 'R') ? "red" : "blue");
    
    for (int i = 0; i < ORE_COUNT; i++) {
        while (1) {
            int pos;
            printf("%s ore %d: ", (ore_type == 'R') ? "Red" : "Blue", i+1);
            
            if (scanf("%d", &pos) != 1) {
                printf("Invalid input. Please enter a number (1-15).\n");
                while (getchar() != '\n');
                continue;
            }
            
            // Convert position to grid coordinates (rows 1-3)
            int row = ((pos - 1) / COLS) + 1;
            int col = (pos - 1) % COLS;
            
            if (pos < 1 || pos > 15) 
                printf("Position must be between 1-15. Try again.\n");
            else if (grid->cells[row][col] != '0') 
                printf("Position already occupied. Try again.\n");
            else 
            {
                grid->cells[row][col] = ore_type;
                if(ore_type == 'R') {
                    grid->red[i].x = col;
                    grid->red[i].y = row;
                } else {
                    grid->blue[i].x = col;
                    grid->blue[i].y = row;
                }
                break;
            }
        }
    }
}

void copy_grid(char (*dest)[5][5], char (*src)[5][5]) {
    memcpy(dest, src, 5 * 5 * sizeof(char));
}

// Function to update a cell in the grid
// Returns true if update was successful, false if coordinates were invalid
bool update_map(Grid *grid, int x, int y, char new_char) {
    // Check if coordinates are within bounds
    if (x < 0 || x >= COLS || y < 0 || y >= ROWS) {
        return false;  // Invalid coordinates
    }
    
    // Update the cell
    grid->cells[y][x] = new_char;  // Note: y comes first for rows, then x for columns
    return true;
}