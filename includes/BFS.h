#ifndef BFS_H
#define BFS_H

#include <stdint.h>
#include <string.h>

#define BFS_ROWS 4
#define BFS_COLS 5
#define MAX_ORES 8
#define MAX_PATH 100  // Adjust based on expected max path length
#define QUEUE_SIZE 3000

// Convert grid coordinates to array indices
#define GRID_TO_ARRAY_X(gx) (gx)
#define GRID_TO_ARRAY_Y(gy) (BFS_ROWS - 1 - gy)

// Convert array indices to grid coordinates
#define ARRAY_TO_GRID_X(ax) (ax)
#define ARRAY_TO_GRID_Y(ay) (BFS_ROWS - 1 - ay)


typedef struct {
    uint8_t x;
    uint8_t y;
} Point;

// Enhanced state tracking structure
typedef struct {
    uint8_t steps;
    uint8_t parent_x;
    uint8_t parent_y;
    uint8_t parent_mask;
} StateInfo;

// BFS queue implementation
typedef struct {
    uint8_t x, y, mask, steps;
} QueueState;

//BFS.c
void enqueue(uint8_t x, uint8_t y, uint8_t mask, uint8_t steps);
QueueState dequeue();
void bfs_init();
void bfs_reconstruct_path(QueueState final);
uint8_t find_ore_path();

//BFS_utils.c
void initialize_bfs_path(void);
const Point* get_bfs_path(void);
uint8_t get_path_length(void);
Point get_path_point(uint8_t index);

#endif