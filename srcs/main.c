#include "../includes/robot.h"

void    print_checks(Grid map)
{
    printf("\nFinal Grid:");
    print_grid(&map);
    Grid fresh;
    copy_grid(&fresh.cells, &map.cells);
    printf("\nCopied Grid:");
    print_grid(&fresh);
    print_ore_positions(&map);
}

int main() {
    Grid            map;
    Robot           robot;
    path            p;
    Position        goal = {0, 2};

    grid_init(&map);
    robot_init(&robot);
    
    //Ask for position of ores and store them in the ore_positions struct
    get_positions(&map, 'R');  // Red ores
    get_positions(&map, 'B');  // Blue ores
    print_checks(map);
    while (1)
    {
        printf("\nEnter new target cell (x y): ");
        int h = scanf("%d %d", &goal.x, &goal.y);
        (void)h;
        a_star(&p, robot.pos, goal, &map);
        int k = convert_path_to_instructions(p.path, p.path_length, &robot);
        printf("Path (length %d):\n", p.path_length);
            for (int i = 0; i < p.path_length; i++) {
                printf("(%d, %d) ", p.path[i].x, p.path[i].y);
            }
        printf("\nMovement instructions:\n");
        for (int i = 0; i < k; i++) {
            printf("%s ->", robot.instructions[i]);
        }
    }
    return 0;
}