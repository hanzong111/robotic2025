#include "../includes/robot.h"
int main() {
    Grid my_grid;
    initialize_grid(&my_grid);
    
    printf("Initial 5×5 Grid (Row 0: Entry, Row 4: Drop-off):");
    print_grid(&my_grid);

    get_positions(&my_grid, 'R');  // Red ores
    get_positions(&my_grid, 'B');  // Blue ores
    
    printf("\nFinal Grid:");
    print_grid(&my_grid);

    return 0;
}