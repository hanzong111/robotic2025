#include "robot.h"

Position get_direction(Direction dir);

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

    for (int j = 0; j < 100; j++) {
        robot->instructions[j] = 0;
    }
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

Chassis_Action* get_Task_List() {
    return Task_List;
}