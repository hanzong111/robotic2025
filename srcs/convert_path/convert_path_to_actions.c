#include "../includes/robot.h"
#include "../includes/BFS.h"

float angle_between_vectors(const Position *a, const Position *b) {
    // Convert to math coordinates (Y increases upward)
    float ax = a->x, ay = -a->y;  // Flip Y for calculation
    float bx = b->x, by = -b->y;  // Flip Y for calculation
    
    float dot = ax * bx + ay * by;
    float cross = ax * by - ay * bx;
    
    // Calculate angle
    float angle = atan2f(cross, dot);
    return angle;
}

void add_turns(Robot *r, Direction dir, int *instruction_count) {
    Position current_dir = r->dir;
    Position target_dir = DIRECTION_VECTORS[dir];
    
    // Calculate signed angle between directions using atan2
    float angle = angle_between_vectors(&current_dir, &target_dir);
    
    // Normalize angle to [-π, π]
    angle = atan2(sin(angle), cos(angle));

    // Check for U-turn (180° ± epsilon) FIRST
    if (fabs(fabs(angle) - M_PI) < ANGLE_EPSILON) {
        r->instructions[(*instruction_count)++] = U_TURN;
        robot_facing(r, dir);
        return;
    }

    // Determine turn direction with proper thresholding
    if (angle > ANGLE_EPSILON) {
        r->instructions[(*instruction_count)++] = TURN_RIGHT;
    } else if (angle < -ANGLE_EPSILON) {
        r->instructions[(*instruction_count)++] = TURN_LEFT;
    }

    // Update direction after turns
    robot_facing(r, dir);
}

void copyPointsToPositions(const Point* source, Position* destination, size_t count) {
    for (size_t i = 0; i < count; ++i) {
        // Convert uint8_t values to int (implicit conversion)
        destination[i].x = source[i].x;
        destination[i].y = source[i].y;
    }
}

int convert_path_to_actions(const Point *path_1, int path_length, Robot *r, Grid *grid) {
    if (path_length < 2) return 0;  // No movement needed
    
    int         instruction_count = 0;
    Position    path[MAX_PATH];
    
    for (int i = 0; i < path_length; ++i) {
        // Convert uint8_t values to int (implicit conversion)
        path[i].x = path_1[i].x;
        path[i].y = path_1[i].y;
    }
    
    for (int i = 0; i < path_length - 1; i++) {
        Position current = path[i];
        Position next = path[i + 1];
        
        // Calculate movement direction
        int dx = next.x - current.x;
        int dy = next.y - current.y;

        if (dx == 1) //GO_RIGHT
            add_turns(r, RIGHT, &instruction_count);
        else if (dx == -1) //GO_LEFT
            add_turns(r, LEFT, &instruction_count);
        else if (dy == 1) //GO_DOWN
            add_turns(r, DOWN, &instruction_count);
        else if (dy == -1) //GO_UP
            add_turns(r, UP, &instruction_count);
        else {
            // Invalid movement (diagonal or no movement)
            return -1;
        }

        if(grid->cells[next.y][next.x] == 'R')
        {
            r->instructions[instruction_count] = GRAB_RED;
            grid->cells[next.y][next.x] = '0';
        }
        else if (grid->cells[next.y][next.x] == 'B' )
        {
            r->instructions[instruction_count] = GRAB_BLUE;
            grid->cells[next.y][next.x] = '0';
        }
        else if (grid->cells[next.y][next.x] == 'D')
        {
            r->instructions[instruction_count] = DROP;
            instruction_count++;
            r->instructions[instruction_count] = MOVE_BACKWARD;
        }
        else
            r->instructions[instruction_count] = MOVE_FOWARD;
        instruction_count++;
    }
    r->instructions[instruction_count] = END;
    return instruction_count;
}