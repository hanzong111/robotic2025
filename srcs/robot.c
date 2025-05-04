#include "../includes/robot.h"

Position get_direction(Direction dir);
void    robot_turn(Robot *robot, Direction dir);

void    robot_init(Robot *robot) {
    robot->pos.x = 2;
    robot->pos.y = 0;

    robot->old_pos.x = -1;
    robot->old_pos.y = -1;

    robot->dir = get_direction(DOWN);
    robot->facing = get_direction(DOWN);

    for (size_t i = 0; i < sizeof(robot->basket); i++) {
        robot->basket[i] = EMPTY;
    }

    for (int j = 0; j < 50; j++) {
        robot->instructions[j] = 0;
    }
}

void    update_robot_position(Robot *robot, int x, int y) {
    robot->pos.x = x;
    robot->pos.y = y;
}

// Function to update robot's basket
bool update_robot_basket(Robot *robot, int index, char value) {
    // Validate index
    if (index < 0 || index >= BASKET_SIZE) {
        return false;
    }

    // Validate value (must be 'R', 'B', or EMPTY)
    if (value != 'R' && value != 'B' && value != EMPTY) {
        return false;
    }

    // Update the basket slot
    robot->basket[index] = value;
    return true;
}

// Use this function to find 'R' or 'B' or EMPTY in the basket
int find_from_basket(const Robot *robot, char value) {
    for (int i = 0; i < BASKET_SIZE; i++) {
        if (robot->basket[i] == value) {
            return i;
        }
    }
    return -1;  // Value not found in basket
}

// Define direction vectors at program start
const Position  DIRECTION_VECTORS[NUM_DIRECTIONS] = {
    [LEFT]  = {-1, 0},  // Left direction
    [RIGHT] = {1, 0},   // Right direction
    [UP]    = {0, 1},  // Up direction
    [DOWN]  = {0, -1}    // Down direction
};

Position get_direction(Direction dir) {
    return DIRECTION_VECTORS[dir];
}

void    robot_facing(Robot *robot, Direction dir) {
    robot->dir = get_direction(dir);
}

Chassis_Action* get_Task_List(Robot *robot) {
    return robot->instructions;
}