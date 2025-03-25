#include "../includes/robot.h"

void initialize_grid(Grid *grid) {
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
}

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
                break;
            }
        }
    }
}