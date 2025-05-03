#include "../includes/robot.h"
#include "../includes/A_star.h"
#include "../includes/BFS.h"

// void    print_checks(Grid map, Robot robot)
// {
//     (void)robot;
//     printf("\nFinal Grid:");
//     print_grid(&map);
//     Grid fresh;
//     copy_grid(&fresh.cells, &map.cells);
//     printf("\nCopied Grid:");
//     print_grid(&fresh);
    //print_ore_positions(&map);
    // while (1)
    // {
    //     printf("\nEnter new target cell (x y): ");
    //     int h = scanf("%d %d", &goal.x, &goal.y);
    //     (void)h;
    //     a_star(&p, robot.pos, goal, &map);
    //     int k = convert_path_to_actions(p.path, p.path_length, &robot);
    //     printf("Path (length %d):\n", p.path_length);
    //         for (int i = 0; i < p.path_length; i++) {
    //             printf("(%d, %d) ", p.path[i].x, p.path[i].y);
    //         }
    //     printf("\nMovement instructions:\n");
    //     action_executor(k, &robot);
    // }
// }

// void    action_executor(int k, Robot *robot){
//     for (int i = 0; i < k; i++) {
//         if(robot->instructions[i] == TURN_LEFT) 
//             printf("TURN_LEFT ->");
//         else if(robot->instructions[i] == TURN_RIGHT) 
//             printf("TURN_RIGHT ->");
//         else if(robot->instructions[i] == MOVE) 
//             printf("MOVE_STRAIGHT ->");
//     }
// }

// int main() {
//     // Position        goal = {0, 2};
//     Grid            map;
//     Robot           robot;
//     // path            p;

//     grid_init(&map);
//     robot_init(&robot);
    
//     //Ask for position of ores and store them in the ore_positions struct
//     get_positions(&map, 'R');  // Red ores
//     get_positions(&map, 'B');  // Blue ores
//     print_checks(map, robot);

//     // Start BFS from the robot's initial position
//     find_ore_path(&map);

//     uint8_t length = get_path_length();
//     const Point* path = get_bfs_path();

//     printf("Shortest Path to get all ores:\n");
//     for(uint8_t i = 0; i < length; i++) {
//         Point path = get_path_point(i);
//         printf("(%d, %d) ", path.x, path.y);
//     }
//     (void)path;
//     return 0;
// }

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define TEST_ROWS 3
#define TEST_COLS 5
#define ORE_COUNT 4

void blue_callback(int *indices, int k, void *data);

typedef struct {
    char cells[TEST_ROWS + 1][TEST_COLS];  // rows 1-3, cols 0-4
    Position red[ORE_COUNT];
    Position blue[ORE_COUNT];
} test_Grid;

// Combination generator callback type
typedef void (*combination_callback)(int *indices, int k, void *data);

// Data structure to pass through combination callbacks
typedef struct {
    test_Grid *grid;
    int *positions;
    int *current_red;
    void (*process)(test_Grid *);
} CallbackData;

void generate_combinations(int n, int k, combination_callback callback, void *data) {
    int *combination = malloc(k * sizeof(int));
    for (int i = 0; i < k; i++) combination[i] = i;

    while (1) {
        callback(combination, k, data);

        int i = k - 1;
        while (i >= 0 && combination[i] == n - k + i) i--;

        if (i < 0) break;

        combination[i]++;
        for (int j = i + 1; j < k; j++)
            combination[j] = combination[j - 1] + 1;
    }

    free(combination);
}

void initialize_grid(test_Grid *grid) {
    for (int row = 1; row <= TEST_ROWS; row++)
        for (int col = 0; col < TEST_COLS; col++)
            grid->cells[row][col] = '0';
}

void red_callback(int *indices, int k, void *data) {
    (void)k;
    CallbackData *cb_data = (CallbackData *)data;
    int red_pos[ORE_COUNT];
    for (int i = 0; i < ORE_COUNT; i++)
        red_pos[i] = cb_data->positions[indices[i]];

    // Calculate remaining positions for blue
    int remaining[15 - ORE_COUNT];
    int rem_count = 0;
    for (int i = 0; i < 15; i++) {
        int pos = cb_data->positions[i];
        bool is_red = false;
        for (int j = 0; j < ORE_COUNT; j++) {
            if (pos == red_pos[j]) {
                is_red = true;
                break;
            }
        }
        if (!is_red) remaining[rem_count++] = pos;
    }

    // Generate blue combinations from remaining positions
    CallbackData blue_cb_data = {
        .grid = cb_data->grid,
        .positions = remaining,
        .current_red = red_pos,
        .process = cb_data->process
    };
    generate_combinations(11, ORE_COUNT, blue_callback, &blue_cb_data);
}

void blue_callback(int *indices, int k, void *data) {
    (void)k;
    CallbackData *cb_data = (CallbackData *)data;
    test_Grid *grid = cb_data->grid;
    initialize_grid(grid);

    // Place red ores
    for (int i = 0; i < ORE_COUNT; i++) {
        int pos = cb_data->current_red[i];
        int row = ((pos - 1) / TEST_COLS) + 1;
        int col = (pos - 1) % TEST_COLS;
        grid->cells[row][col] = 'R';
        grid->red[i].x = col;
        grid->red[i].y = row;
    }

    // Place blue ores
    for (int i = 0; i < ORE_COUNT; i++) {
        int pos = cb_data->positions[indices[i]];
        int row = ((pos - 1) / TEST_COLS) + 1;
        int col = (pos - 1) % TEST_COLS;
        grid->cells[row][col] = 'B';
        grid->blue[i].x = col;
        grid->blue[i].y = row;
    }

    // Process the current grid configuration
    cb_data->process(grid);
}

void process_all_combinations(test_Grid *grid, void (*process)(test_Grid *)) {
    int positions[15];
    for (int i = 0; i < 15; i++) positions[i] = i + 1;

    CallbackData cb_data = {
        .grid = grid,
        .positions = positions,
        .process = process
    };

    generate_combinations(15, ORE_COUNT, red_callback, &cb_data);
}

void copy_test_grid(const test_Grid *src, Grid *dest) {
    
    // Copy test_Grid rows 1-3 to Grid rows 2-4
    for (int src_row = 1; src_row <= TEST_ROWS; src_row++) {
        int dest_row = src_row + 1;  // Shift rows down by 1
        for (int col = 0; col < TEST_COLS; col++) {
            dest->cells[dest_row - 1][col] = src->cells[src_row][col];
        }
    }
}

void print_ore_positions_1(const Grid *grid) {
    // Print red ores
    printf("Red Ore Positions:\n");
    for (int i = 0; i < ORE_COUNT; i++) {
        printf("  Ore %d: [%d, %d]\n", 
               i + 1,
               grid->red[i].x,
               grid->red[i].y);  // Convert to 1-based column
    }

    // Print blue ores
    printf("\nBlue Ore Positions:\n");
    for (int i = 0; i < ORE_COUNT; i++) {
        printf("  Ore %d: [%d, %d]\n", 
               i + 1,
               grid->blue[i].x,
               grid->blue[i].y);  // Convert to 1-based column
    }
}

// Example processing function
void example_process(test_Grid *grid) {
    Grid            map;

    grid_init(&map);
    copy_test_grid(grid, &map);
    map.blue[0].y = grid->blue[0]. y;
    map.blue[1].y = grid->blue[1].y;
    map.blue[2].y = grid->blue[2].y;
    map.blue[3].y = grid->blue[3].y;
    map.red[0].y = grid->red[0].y;
    map.red[1].y = grid->red[1].y;
    map.red[2].y = grid->red[2].y;
    map.red[3].y = grid->red[3].y;
    map.blue[0].x = grid->blue[0].x;
    map.blue[1].x = grid->blue[1].x;
    map.blue[2].x = grid->blue[2].x;
    map.blue[3].x = grid->blue[3].x;
    map.red[0].x = grid->red[0].x;
    map.red[1].x = grid->red[1].x;
    map.red[2].x = grid->red[2].x;
    map.red[3].x = grid->red[3].x;
    // print_grid(&map);
    // print_ore_positions_1(&map);
    //exit(0);
    if (find_ore_path(&map) == 2)
    {
            print_grid(&map);
            print_ore_positions_1(&map);
    }

    // if(find_ore_path(&map) == 1)
    // {
    //     printf(GREEN);
    //     printf("OK!\n");
    //     printf("\033[0m\n");
    // }
    // else
    // {
    //     printf(RED);
    //     printf("Error!\n");
    //     printf("\033[0m\n");
    //     print_grid(&map);
    //     print_ore_positions_1(&map);
    //     exit(0);
    // }
    static int count = 0;
    ++count;
    if (count == 450450)
        printf("Processed %d configurations...\n", count);
}

int main() {
    test_Grid grid;
    process_all_combinations(&grid, example_process);
    return 0;
}