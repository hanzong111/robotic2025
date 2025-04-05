# include "../includes/BFS.h"
# include  "../includes/robot.h"

// Example ore positions (modify as needed)
Point ores[MAX_ORES];
int8_t ore_index[BFS_ROWS][BFS_COLS];  // Ore position mapping
QueueState queue[QUEUE_SIZE];
uint16_t q_front = 0, q_rear = 0;

// Path storage
Point bfs_path[MAX_PATH];
uint8_t bfs_path_length = 0;
static uint8_t bfs_start_x;
static uint8_t bfs_start_y;

void initialize_bfs_path(void) {
    // Reset path storage
    memset(bfs_path, 0, sizeof(bfs_path));
    bfs_path_length = 0;
}

const Point* get_bfs_path(void) {
    return bfs_path;
}

uint8_t get_path_length(void) {
    return bfs_path_length;
}

Point get_path_point(uint8_t index) {
    if(index < bfs_path_length) {
        return bfs_path[index];
    }
    // Return invalid point for error handling
    return (Point){0xFF, 0xFF};
}

void enqueue(uint8_t x, uint8_t y, uint8_t mask, uint8_t steps) {
    if(q_rear < QUEUE_SIZE) {
        queue[q_rear++] = (QueueState){x, y, mask, steps};
    }
}

QueueState dequeue() {
    return queue[q_front++];
}

static StateInfo visited[BFS_ROWS][BFS_COLS][1 << MAX_ORES];

void print_ore_grid() {
    printf("\n=== Ore Grid (Array Indices) ===\n");
    for(int x=0; x<BFS_ROWS; x++) {
        printf("[%d] ", x);
        for(int y=0; y<BFS_COLS; y++) {
            if(ore_index[x][y] != -1) {  // Note: x first!
                printf("%2d ", ore_index[x][y]);
            } else {
                printf(" . ");
            }
        }
        printf("\n");
    }
    printf("===============================\n");
    
}

void bfs_init(Grid *grid) {
    // Convert grid start (2,0) to array indices
    bfs_start_x = GRID_TO_ARRAY_X(2);
    bfs_start_y = GRID_TO_ARRAY_Y(0);

    printf("Extracting blue ore positions...\n");
    // Extract blue ores (first 4 positions)
    for(uint8_t i = 0; i < ORE_COUNT; i++) {
        ores[i] = (Point){
            .x = grid->blue[i].x,
            .y = grid->blue[i].y
        };
    }
    printf("Extracting red ore positions...\n");
    // Extract red ores (next 4 positions)
    for(uint8_t i = 0; i < ORE_COUNT; i++) {
        ores[ORE_COUNT + i] = (Point){
            .x = grid->red[i].x,
            .y = grid->red[i].y
        };
    }

    for(int i=0; i<MAX_ORES; i++) {
        printf("Ore %d at (%d,%d)\n", i, ores[i].x, ores[i].y);
    }
    
    memset(ore_index, -1, sizeof(ore_index));
    for(int i=0; i<MAX_ORES; i++) {
        uint8_t x = ores[i].x;
        uint8_t y = ores[i].y;
        ore_index[y][x] = i;
    }

    print_ore_grid();
    
    // Initialize visited states
    for(int x = 0; x < BFS_ROWS; x++) {
        for(int y = 0; y < BFS_COLS; y++) {
            for(int mask = 0; mask < (1 << MAX_ORES); mask++) {
                visited[x][y][mask].steps = 0xFF;  // 255 = unvisited
                visited[x][y][mask].parent_x = 0xFF;
                visited[x][y][mask].parent_y = 0xFF;
                visited[x][y][mask].parent_mask = 0xFF;
            }
        }
    }
    printf("Ready!!!\n");
}

void bfs_reconstruct_path(QueueState final) {
    uint8_t x = final.y;  // Array indices
    uint8_t y = final.x;
    uint8_t mask = final.mask;
    bfs_path_length = 0;
    printf("Here\n");
    // Clear previous path
    memset(bfs_path, 0, sizeof(bfs_path));

    while(1) {
        // Convert to grid coordinates before storing
        bfs_path[bfs_path_length] = (Point){
            .x = y,
            .y = x,
        };
        bfs_path_length++;

        // Exit condition: reached start position
        if(x == 0 && y == 2 && mask == 0) break;

        // Safety checks
        if(bfs_path_length >= MAX_PATH || 
           visited[x][y][mask].parent_x == 0xFF ||
           visited[x][y][mask].parent_y == 0xFF) {
            bfs_path_length = 0;
            return;
        }

        // Move to parent state
        StateInfo info = visited[x][y][mask];
        x = info.parent_y;
        y = info.parent_x;
        mask = info.parent_mask;
    }

    // Reverse the path
    for(int i = 0; i < bfs_path_length/2; i++) {
        Point temp = bfs_path[i];
        bfs_path[i] = bfs_path[bfs_path_length-1-i];
        bfs_path[bfs_path_length-1-i] = temp;
    }
}

uint8_t find_ore_path(Grid *grid) {
    bfs_init(grid);
    // Original (array coordinates)
    const int8_t dx[] = {-1, 1, 0, 0};  // Up/Down
    const int8_t dy[] = {0, 0, -1, 1};  // Left/Right
    int m = 0;
    
    // Clear queue and visited states
    q_front = q_rear = 0;
    memset(visited, 0xFF, sizeof(visited));  // Reset to 0xFF

    // Initialize starting state
    enqueue(2, 0, 0, 0);
    visited[0][2][0].steps = 0;

    while(q_front < q_rear) {
        m++;
        QueueState current = dequeue();
        // printf("\nCurrent State:\n");
        // printf("Position: (%d, %d) | Steps: %d\n", current.x, current.y, current.steps);
        // printf("Mask: 0b");
        // // Print 8-bit binary representation (for MAX_ORES=8)
        // for(int i = 7; i >= 0; i--) {
        //     printf("%d", (current.mask >> i) & 1);
        // }
        // printf(" (0x%02x)\n", current.mask);
        // printf("Collected Ores: ");
        // if(current.mask == 0) {
        //     printf("None");
        // } else {
        //     for(int i = 0; i < MAX_ORES; i++) {
        //         if(current.mask & (1 << i)) {
        //             printf("%d ", i);
        //         }
        //     }
        // }
        // printf("\n----------------------------\n");

        // // // Check completion condition
        if(current.mask == (1 << MAX_ORES) - 1) {
            printf("All ores collected!\n");
            printf("%d iterations !!!\n", m);
            bfs_reconstruct_path(current);
            return 1;
        }

        // Explore neighbors
        for(int i=0; i<4; i++) {
            int16_t nx = current.y + dy[i];
            int16_t ny = current.x + dx[i];
            
            if(nx >=0 && nx < BFS_ROWS && ny >=0 && ny < BFS_COLS) {
                uint8_t new_mask = current.mask;
                int8_t ore_id = ore_index[nx][ny];
                
                if(ore_id != -1) {
                    new_mask |= (1 << ore_id);
                }

                if(current.steps + 1 < visited[nx][ny][new_mask].steps) {
                // Update state only if we found a better path to this (x,y,mask) combo
                visited[nx][ny][new_mask].steps = current.steps + 1;
                visited[nx][ny][new_mask].parent_x = current.x;
                visited[nx][ny][new_mask].parent_y = current.y;
                visited[nx][ny][new_mask].parent_mask = current.mask;
                enqueue(ny, nx, new_mask, current.steps + 1);
}
            }
        }
    }
    printf("%d iterations !!!\n", m);
    printf("No path found!\n");
    return 0;
}