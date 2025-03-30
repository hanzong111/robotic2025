#include "../includes/robot.h"

int convert_path_to_actions(const Position *path, int path_length, Robot *r) {
    if (path_length < 2) return 0;  // No movement needed
    
    int instruction_count = 0;
    
    for (int i = 0; i < path_length - 1; i++) {
        Position current = path[i];
        Position next = path[i + 1];
        
        // Calculate movement direction
        int dx = next.x - current.x;
        int dy = next.y - current.y;

        if (dx == 1) {
            r->instructions[instruction_count] = GO_RIGHT;
        } else if (dx == -1) {
            r->instructions[instruction_count] = GO_LEFT;
        } else if (dy == 1) {
            r->instructions[instruction_count] = GO_DOWN;
        } else if (dy == -1) {
            r->instructions[instruction_count] = GO_UP;
        } else {
            // Invalid movement (diagonal or no movement)
            return -1;
        }
        
        instruction_count++;
        
        // Safety check
        if (instruction_count >= 25) break;
    }
    
    return instruction_count;
}