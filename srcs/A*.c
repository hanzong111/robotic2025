#include "../includes/robot.h"

// Directions: Up, Down, Left, Right (4-way movement)
const int dx[4] = {0, 1, 0, -1};
const int dy[4] = {-1, 0, 1, 0};

// Manhattan distance (for 4-directional movement)
int heuristic(Position a, Position b) {
    return abs(a.x - b.x) + abs(a.y - b.y);
}

void    a_star_init(path *p)
{
    memset(p->open_list, 0, sizeof(Node) * ROWS * COLS);
    memset(p->path, 0, sizeof(Position) * ROWS * COLS);
    //Initialize the g_score cells to infinity
    for (int y = 0; y < 5; y++)
        for (int x = 0; x < 5; x++)
            p->g_score[y][x] = INT_MAX;
    //Initialize the closed_list cells to false
    for (int y = 0; y < 5; y++)
        for (int x = 0; x < 5; x++)
            p->closed_list[y][x] = false;
    //Initialize the came_from cells to -1
    for (int y = 0; y < 5; y++)
        for (int x = 0; x < 5; x++)
            p->came_from[y][x] = (Position){-1, -1};
    p->open_size = 0;
    p->path_length = 0;
    p->is_target = false;
}

bool    reconstruct_path(path *p, Position start, Position goal) {
    Position current = goal;

    // Edge case: start and goal are the same
    if (current.x == start.x && current.y == start.y) {
        p->path[0] = current;
        return true;
    }

    // Trace back from goal to start
    while (true) {
        // Add current position to path
        p->path[p->path_length++] = current;

        // Prevent infinite loops (sanity check for grid size)
        if (p->path_length >= 25) {
            return false;  // Error: path too long
        }

        // Move to parent node
        current = p->came_from[current.y][current.x];

        // Stop when we reach the start position
        if (current.x == start.x && current.y == start.y) {
            p->path[p->path_length++] = current;  // Add the start position
            break;
        }
    }

    // Reverse the path to get start -> goal order
    for (int i = 0; i < p->path_length / 2; i++) {
        Position temp = p->path[i];
        p->path[i] = p->path[p->path_length - 1 - i];
        p->path[p->path_length - 1 - i] = temp;
    }

    return true;
}

void a_star(path *p, Position start, Position goal, Grid *grid) {
    // Initialize
    a_star_init(p);

    // Add start node
    p->g_score[start.y][start.x] = 0;
    int h_start = heuristic(start, goal);
    open_list_push((Node){start, p->g_score[start.y][start.x] + h_start}, p);

    while (!open_list_is_empty(p->open_size)) {
        Node current = open_list_pop(p);
        Position pos = current.pos;

        if (pos.x == goal.x && pos.y == goal.y) {
            if(!reconstruct_path(p, start, goal))
                printf("Error: Path too long\n");
            return;
        }

        if (p->closed_list[pos.y][pos.x]) continue;
        p->closed_list[pos.y][pos.x] = true;

        /* grid->cells[neighbor.y][neighbor.x] == 'D' is for blocking the dropoff path because on the map it cannot be tranversed */
        /* added !p->is_target so that A* knows to skip ores/dropoff except targeted ores/dropoff */
        for (int i = 0; i < 4; i++) {
            Position neighbor = {pos.x + dx[i], pos.y + dy[i]};
            p->is_target = (neighbor.x == goal.x && neighbor.y == goal.y);
            // Skip invalid/blocked cells
            if (!p->is_target && (neighbor.x < 0 || neighbor.x >= 5 || 
                neighbor.y < 0 || neighbor.y >= 5 ||
                (grid->cells[neighbor.y][neighbor.x] == 'R') || 
                (grid->cells[neighbor.y][neighbor.x] == 'B') ||
                (grid->cells[neighbor.y][neighbor.x] == 'D'))) {
                continue;
            }

            // Calculate new g-score
            int new_g = p->g_score[pos.y][pos.x] + 1;
            if (new_g < p->g_score[neighbor.y][neighbor.x]) {
                p->came_from[neighbor.y][neighbor.x] = pos;
                p->g_score[neighbor.y][neighbor.x] = new_g;
                int f_new = new_g + heuristic(neighbor, goal);
                open_list_push((Node){neighbor, f_new}, p);
            }
        }
    }
}