#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "task.h"

#define MAX_PRIORITY 10
#define MIN_PRIORITY 1

struct node* priority_queue[MAX_PRIORITY] = {NULL};

// Add a task to the list
void add(char* name, int priority, int burst) {
    Task* temp = malloc(sizeof(Task));
    temp->name = name;
    temp->priority = priority;
    temp->burst = burst;
    temp->tid = 0;
    insert(&priority_queue[priority], temp);
}

void schedule() {

    int time = 0;
    int dispatch_time = -1;
    int is_empty = 0;
    int index = MAX_PRIORITY;

    while (!is_empty) {
        // Find the highest non-empty priority queue
        while (index >= MIN_PRIORITY && priority_queue[index] == NULL) {
            index--;
        }

        if (index < MIN_PRIORITY) {
            is_empty = 1;
            continue;
        }

        // Process all tasks in the current priority queue
        while (priority_queue[index] != NULL) {
            struct node* temp = priority_queue[index];

            // Move to last element in the linked list
            while (temp->next != NULL) {
                temp = temp->next;
            }

            dispatch_time++;
            time += temp->task->burst;
            printf("Running task = [%s] [%d] [%d] for %d units.\n",
                   temp->task->name, temp->task->priority, temp->task->burst,
                   temp->task->burst);
            printf("        Time is now: %d\n", time);

            delete (&priority_queue[index], temp->task);
        }
    }

    dispatch_time += time;

    printf("CPU Utilization: %.2f%%\n", ((float)time / dispatch_time) * 100);
}
