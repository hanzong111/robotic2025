#include "../../includes/robot.h"

bool open_list_is_empty(int open_size)
{
    return open_size == 0;
}

int open_list_find(Position pos, int open_size, Node *open_list)
{
    for (int i = 0; i < open_size; i++) {
        if (open_list[i].pos.x == pos.x && open_list[i].pos.y == pos.y) {
            return i;
        }
    }
    return -1;
}

void open_list_push(Node node, path *p) {
    // Check for duplicates and replace if better f-value
    int existing_index = open_list_find(node.pos, p->open_size, p->open_list);
    if (existing_index != -1) {
        if (node.f < p->open_list[existing_index].f) {
            // Remove existing node
            for (int i = existing_index; i < p->open_size - 1; i++) {
                p->open_list[i] = p->open_list[i + 1];
            }
            p->open_size--;
        } else {
            // Existing node is better, skip adding
            return;
        }
    }

    // Insert in sorted order (ascending f-value)
    int insert_pos = 0;
    while (insert_pos < p->open_size && p->open_list[insert_pos].f < node.f) {
        insert_pos++;
    }

    // Shift elements to make space
    for (int i = p->open_size; i > insert_pos; i--) {
        p->open_list[i] = p->open_list[i - 1];
    }

    // Insert new node
    p->open_list[insert_pos] = node;
    p->open_size++;
}

Node open_list_pop(path *p) {
    Node node = p->open_list[0];
    // Shift remaining elements
    for (int i = 0; i < p->open_size - 1; i++) {
        p->open_list[i] = p->open_list[i + 1];
    }
    p->open_size--;
    return node;
}