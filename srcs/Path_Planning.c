#include "../includes/robot.h"
#include "../includes/BFS.h"
#include "../includes/add_dropoff_path.h"

Chassis_Action* Task_List = NULL; 

void    print_Task_List(Chassis_Action *instructions){
    int k = 0;

    while (instructions[k] != END && k < 100) {
        k++;
    }
    k++; // Increment to include END
    for (int i = 0; i < k; i++) {
        if(instructions[i] == TURN_LEFT) 
            printf("TURN_LEFT ->");
        else if(instructions[i] == TURN_RIGHT) 
            printf("TURN_RIGHT ->");
        else if(instructions[i] == MOVE_FOWARD) 
            printf("MOVE_STRAIGHT ->");
        else if(instructions[i] == GRAB_RED)
        {
            printf(RED);
            printf("GRAB RED ->");
            printf("\033[0;97m ");
        }
        else if(instructions[i] == GRAB_BLUE)
        {
            printf(BLUE);
            printf("GRAB BLUE ->");
            printf("\033[0;97m ");
        }
        else if(instructions[i] == DROP)
        {
            printf(GREEN);
            printf("DROP ->");
            printf("\033[0;97m ");
        }
        else if (instructions[i] == U_TURN)
            printf("U_TURN ->");
        else if (instructions[i] == MOVE_BACKWARD)
            printf("MOVE_BACKWARD ->");
        else if (instructions[i] == END)
            printf("END\n");
    }
}

void    Path_Planning()
{
    Grid map;
    Robot robot;
    Chassis_Action *instructions;

    grid_init(&map);
    robot_init(&robot);

    // Print the grid and ore positions
    //Ask for position of ores and store them in the ore_positions struct
    // get_positions(&map, 'R');  // Red ores
    // get_positions(&map, 'B');  // Blue ores
    //Manual Test
    int red_ores[4] = {1, 2, 3, 4};
    int blue_ores[4] = {5, 6, 7, 8};
    //store them in the ore_positions struct
    get_positions(&map, red_ores, blue_ores);  // Red ores
    print_grid(&map);
    print_ore_positions(&map);
    
    // Start BFS from the robot's initial position
    find_ore_path(&map);
    // Add drop-off path
    add_dropoff_path();
    convert_path_to_actions(get_bfs_path(), get_path_length(), &robot, &map);
    // Print the path
    Task_List = robot.instructions;
    instructions = get_Task_List();
    // Print the task list
    print_Task_List(instructions);
    for(int i = 0; i < 8; i++)
    {
        printf("%c", robot.ore_sequence[i]);
    }
}