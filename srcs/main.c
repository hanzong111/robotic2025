#include "../includes/robot.h"
#include "../includes/BFS.h"

void    print_checks(Grid map, Robot robot)
{
    (void)robot;
    printf("\nFinal Grid:");
    print_grid(&map);
    Grid fresh;
    copy_grid(&fresh.cells, &map.cells);
    printf("\nCopied Grid:");
    print_grid(&fresh);
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
}

void    action_executor(int k, Robot *robot){
    for (int i = 0; i < k; i++) {
        if(robot->instructions[i] == TURN_LEFT) 
            printf("TURN_LEFT ->");
        else if(robot->instructions[i] == TURN_RIGHT) 
            printf("TURN_RIGHT ->");
        else if(robot->instructions[i] == MOVE) 
            printf("MOVE_STRAIGHT ->");
    }
}

int main() {
    // Position        goal = {0, 2};
    Grid            map;
    Robot           robot;
    // path            p;

    grid_init(&map);
    robot_init(&robot);
    
    //Ask for position of ores and store them in the ore_positions struct
    get_positions(&map, 'R');  // Red ores
    get_positions(&map, 'B');  // Blue ores
    print_checks(map, robot);

    // Start BFS from the robot's initial position
    find_ore_path(&map);

    uint8_t length = get_path_length();
    const Point* path = get_bfs_path();

    printf("Shortest Path to get all ores:\n");
    for(uint8_t i = 0; i < length; i++) {
        Point path = get_path_point(i);
        printf("(%d, %d) ", path.x, path.y);
    }
    (void)path;
    return 0;
}