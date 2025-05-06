#include "robot.h"
#include "BFS.h"
#include "fire_source.h"

void    Path_Planning()
{
    Grid map;
    Robot robot;
    FireSource *fire_source = &fireSources;

    grid_init(&map);
    robot_init(&robot);

    //store them in the ore_positions struct
    get_positions(&map, fire_source);  // Red ores
    
    // Start BFS from the robot's initial position
    find_ore_path(&map);
    // Add drop-off path
    add_dropoff_path();
    convert_path_to_actions(get_bfs_path(), get_path_length(), &robot, &map);

    Task_List = robot.instructions;
}