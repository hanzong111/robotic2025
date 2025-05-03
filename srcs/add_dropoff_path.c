#include "../includes/BFS.h"
#include "../includes/add_dropoff_path.h"
#include "../includes/robot.h"
#include <stdio.h>

int    add_left_or_right(Point* path, int index)
{
    if (path[index - 1].x == 2)
    {
        Point current = path[index - 2];
        Point next = path[index - 1];
        
        // Calculate movement direction
        int dx = next.x - current.x;
        int dy = next.y - current.y;

        if (dx == 1 || dy == 1 || dy == -1) //FACE_RIGHT or DOWN or UP
        {
            path[index].x = next.x + 1;
            path[index].y = next.y;
        }
        else if (dx == -1) //FACE_LEFT
        {
            path[index].x = next.x - 1;
            path[index].y = next.y;
        }
        index++;
    }
    return (index);
}

int move_to_first_dropoff(Point* path, int index)
{
    while(path[index - 1].y != 4)
    {
        path[index].x = path[index - 1].x;
        path[index].y = path[index - 1].y + 1;
        index++;
    }
    return (index);
}

void    move_to_remaining_dropoff(Point* path, int index)
{
    if (path[index - 1].x == 0 || path[index - 1].x == 1) // Iterate to the right
    {
        for (int k = 0; k <= 2; k++)
        {
            path[index].x = path[index - 1].x;
            path[index].y = path[index - 1].y - 1;
            index++;
            path[index].x = path[index - 1].x + 1;
            path[index].y = path[index - 1].y;
            index++;
            path[index].x = path[index - 1].x;
            path[index].y = path[index - 1].y + 1;
            index++;
        }
    }
    else if (path[index - 1].x == 3 || path[index - 1].x == 4) // Iterate to the left 
    {
        for (int k = 0; k <= 2; k++)
        {
            path[index].x = path[index - 1].x;
            path[index].y = path[index - 1].y - 1;
            index++;
            path[index].x = path[index - 1].x - 1;
            path[index].y = path[index - 1].y;
            index++;
            path[index].x = path[index - 1].x;
            path[index].y = path[index - 1].y + 1;
            index++;
        }
    }
   
}

void    add_dropoff_path()
{
    Point* path = get_bfs_path();
    int index = 0;

    for (; index < MAX_PATH; index++) {
        if (path[index].x == EMPTY_POSITION && path[index].y == EMPTY_POSITION) 
            break;
    }
    index = add_left_or_right(path, index);
    index = move_to_first_dropoff(path, index);
    move_to_remaining_dropoff(path, index);
}
