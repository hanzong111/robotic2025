#include "robot.h"
#include "BFS.h"

Chassis_Action* Task_List = NULL; 

void    Path_Planning()
{
    Grid map;
    Robot robot;

    grid_init(&map);
    robot_init(&robot);

    int *red_ores = /* Zhiyao 那边的Red Array*/;
    int *blue_ores = /* Zhiyao 那边的Blue Array*/;

    //store them in the ore_positions struct
    get_positions(&map, red_ores, blue_ores);  // Red ores
    
    // Start BFS from the robot's initial position
    find_ore_path(&map);
    // Add drop-off path
    add_dropoff_path();
    convert_path_to_actions(get_bfs_path(), get_path_length(), &robot, &map);

    Task_List = robot.instructions;
}