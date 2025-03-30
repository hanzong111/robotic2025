#include "../includes/robot.h"

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

float vector_to_angle(const Position *dir) {
    return atan2f(dir->y, dir->x);
}

void add_turns(Robot *r, Direction dir, int *instruction_count) {
    Position target_dir = DIRECTION_VECTORS[dir];
    float angle = angle_between_vectors(&r->dir, &target_dir);

    // Handle 180° turn first
    if (fabs(fabs(angle) - M_PI) < ANGLE_EPSILON) {
        r->instructions[(*instruction_count)++] = U_TURN;
        r->dir = target_dir;
        return;
    }

    // Determine turn direction
    if (angle > ANGLE_EPSILON) {
            r->instructions[(*instruction_count)++] = TURN_RIGHT;
    } 
    else if (angle < -ANGLE_EPSILON) {
            r->instructions[(*instruction_count)++] = TURN_LEFT;
    }
    
    // Update direction after turns
    r->dir = target_dir;
}

int convert_path_to_actions(const Position *path, int path_length, Robot *r) {
    if (path_length < 2) return 0;  // No movement needed
    
    int         instruction_count = 0;
    
    for (int i = 0; i < path_length - 1; i++) {
        Position current = path[i];
        Position next = path[i + 1];
        
        // Calculate movement direction
        int dx = next.x - current.x;
        int dy = next.y - current.y;

        if (dx == 1) //GO_RIGHT
        {
            add_turns(r, RIGHT, &instruction_count);
            r->instructions[instruction_count] = MOVE_STRAIGHT;
        }
        else if (dx == -1) //GO_LEFT
        {
            add_turns(r, LEFT, &instruction_count);
            r->instructions[instruction_count] = MOVE_STRAIGHT;
        }
        else if (dy == 1) //GO_DOWN
        {
            add_turns(r, DOWN, &instruction_count);
            r->instructions[instruction_count] = MOVE_STRAIGHT;
        }
        else if (dy == -1) //GO_UP
        {
            add_turns(r, UP, &instruction_count);
            r->instructions[instruction_count] = MOVE_STRAIGHT;
        }
        else {
            // Invalid movement (diagonal or no movement)
            return -1;
        }
        instruction_count++;
        
        // Safety check
        if (instruction_count >= 50) break;
    }
    
    return instruction_count;
}